#include <wiringx.h>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <span>
#include <unistd.h>

#include "spdlog/spdlog.h"
#include "minmea.h"

#include "GPS.h"
#include "UbxCommands.h"

GPS::GPS(): uartFd(-100),
            nmeaBuffer{},
            nmeaBufferCurrentIdx(0),
            nmeaBufferStartValid(false),
            currentState({}) {
}

void GPS::writeCommandToModule(const std::span<const unsigned char> commandBytes) const {
    for (unsigned char ch: commandBytes) {
        spdlog::trace("Writing command char: {}", ch);
        wiringXSerialPutChar(uartFd, ch);
    }
}

void GPS::setup() {
    spdlog::info("Setting up GPS");

    spdlog::debug("Opening GPS UART with baud 9600");
    uartFd = openUartToGpsModule(9600);
    spdlog::debug("Configure GPS module to increase baud to 115200");
    writeCommandToModule(UBX_CFG_UART_BAUD_115200);

    spdlog::debug("Closing GPS UART to re-open with higher rate");
    wiringXSerialClose(uartFd);
    sleep(1);

    spdlog::debug("Opening GPS UART with baud 115200");
    uartFd = openUartToGpsModule(115200);

    spdlog::info("GPS UART setup completed. FD: {}", uartFd);
}

int GPS::openUartToGpsModule(const unsigned int baudRate) {
    const wiringXSerial_t wiringXSerial = {baudRate, 8, 'n', 1, 'n'};
    int fd;

    if ((fd = wiringXSerialOpen("/dev/ttyS1", wiringXSerial)) < 0) {
        spdlog::critical("Open serial device failed: {}", fd);
        wiringXGC();

        throw std::runtime_error("Can not open GPS UART.");
    }

    return fd;
}

void GPS::shutdown() const {
    spdlog::info("Closing GPS connection");

    wiringXSerialClose(uartFd);

    spdlog::info("GPS connection closed");
}

uint8_t GPS::calculateChecksum(const char *sentence) {
    uint8_t checksum = 0;

    // Skip the initial '$' character
    sentence++;

    // XOR each character until the '*' character or the end of the string
    while (*sentence && *sentence != '*') {
        checksum ^= *sentence;
        sentence++;
    }

    return checksum;
}

std::optional<uint8_t> GPS::extractChecksum(const char *sentence) {
    // Find the '*' character in the sentence
    const char *checksumStr = strchr(sentence, '*');
    if (checksumStr == nullptr || strlen(checksumStr) < 3) {
        // '*' not found or not enough characters after '*' for checksum
        return std::nullopt;
    }

    // Extract the checksum string and convert it to an integer
    return static_cast<uint8_t>(strtol(checksumStr + 1, nullptr, 16));
}

bool GPS::validateChecksum(const char *sentence) {
    const std::optional<uint8_t> extractedChecksum = extractChecksum(sentence);
    if (!extractedChecksum.has_value()) {
        return false;
    }

    const uint8_t calculatedChecksum = calculateChecksum(sentence);

    return calculatedChecksum == extractedChecksum.value();
}

void GPS::readAvailable() {
    auto updates = unprocessedUpdates.value_or(GpsUpdateList{});

    if (int availableBytes = wiringXSerialDataAvail(uartFd);
        availableBytes > 0) {
        spdlog::trace("Received bytes: {}", availableBytes);
        while (availableBytes--) {
            char c = wiringXSerialGetChar(uartFd);
            spdlog::trace("Reading char: {}", c);

            if (c == '$') {
                spdlog::trace("Nmea start detected");
                nmeaBuffer[nmeaBufferCurrentIdx] = '\0';
                if (nmeaBufferStartValid) {
                    if (validateChecksum(nmeaBuffer)) {
                        auto nmeaWithoutNewLineChar = removeNewLineCharacter(nmeaBuffer);
                        spdlog::debug("NMEA: {}", nmeaWithoutNewLineChar);

                        switch (minmea_sentence_id(nmeaBuffer, false)) {
                            case MINMEA_SENTENCE_VTG: {
                                minmea_sentence_vtg frame{};
                                if (minmea_parse_vtg(&frame, nmeaBuffer)) {
                                    currentState.speed = minmea_tofloat(&frame.speed_kph);

                                    updates.emplace_back(SpeedUpdate{
                                        nmeaWithoutNewLineChar,
                                        minmea_tofloat(&frame.speed_kph)
                                    });
                                } else {
                                    spdlog::error("Failed to parse VTG sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            case MINMEA_SENTENCE_GSA: {
                                minmea_sentence_gsa frame{};
                                if (minmea_parse_gsa(&frame, nmeaBuffer)) {
                                    updates.emplace_back(SatellitesUpdate{
                                        nmeaWithoutNewLineChar,
                                        0,
                                        minmea_tofloat(&frame.hdop)
                                    });
                                } else {
                                    spdlog::error("Failed to parse GSA sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            case MINMEA_SENTENCE_GLL: {
                                minmea_sentence_gll frame{};
                                if (minmea_parse_gll(&frame, nmeaBuffer)) {
                                    updates.emplace_back(PositionUpdate{
                                        nmeaWithoutNewLineChar,
                                        false,
                                        minmea_tocoord(&frame.latitude),
                                        minmea_tocoord(&frame.longitude)
                                    });
                                } else {
                                    spdlog::error("Failed to parse GLL sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            case MINMEA_SENTENCE_RMC: {
                                minmea_sentence_rmc frame{};
                                if (minmea_parse_rmc(&frame, nmeaBuffer)) {
                                    currentState.hasFix = frame.valid;

                                    currentState.latitude = minmea_tocoord(&frame.latitude);
                                    currentState.longitude = minmea_tocoord(&frame.longitude);

                                    currentState.day = frame.date.day;
                                    currentState.month = frame.date.month;
                                    currentState.year = frame.date.year;

                                    currentState.hours = frame.time.hours;
                                    currentState.minutes = frame.time.minutes;
                                    currentState.seconds = frame.time.seconds;
                                    currentState.microseconds = frame.time.microseconds;

                                    // This speed is in knots, should be converted at first
                                    // currentState.speed = minmea_tofloat(&frame.speed);

                                    updates.emplace_back(PositionUpdate{
                                        nmeaWithoutNewLineChar,
                                        frame.valid,
                                        minmea_tocoord(&frame.latitude),
                                        minmea_tocoord(&frame.longitude)
                                    });
                                } else {
                                    spdlog::error("Failed to parse RMC sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            case MINMEA_SENTENCE_GGA: {
                                minmea_sentence_gga frame{};
                                if (minmea_parse_gga(&frame, nmeaBuffer)) {
                                    currentState.hasFix = frame.fix_quality > 0;
                                    currentState.fixQuality = frame.fix_quality;
                                    currentState.numberOfSatellites = frame.satellites_tracked;
                                    currentState.hdop = minmea_tofloat(&frame.hdop);

                                    currentState.latitude = minmea_tocoord(&frame.latitude);
                                    currentState.longitude = minmea_tocoord(&frame.longitude);

                                    currentState.hours = frame.time.hours;
                                    currentState.minutes = frame.time.minutes;
                                    currentState.seconds = frame.time.seconds;
                                    currentState.microseconds = frame.time.microseconds;

                                    updates.emplace_back(PositionUpdate{
                                        nmeaWithoutNewLineChar,
                                        frame.fix_quality > 0,
                                        minmea_tocoord(&frame.latitude),
                                        minmea_tocoord(&frame.longitude)
                                    });
                                } else {
                                    spdlog::error("Failed to parse GGA sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            case MINMEA_SENTENCE_GSV: {
                                minmea_sentence_gsv frame{};
                                if (minmea_parse_gsv(&frame, nmeaBuffer)) {
                                    if (frame.msg_nr == 1) {
                                        char gnssType[3];
                                        strncpy(gnssType, nmeaBuffer + 1, 2);
                                        gnssType[2] = '\0';

                                        if (auto gnssTypeStr = std::string(gnssType); gnssTypeStr == "GP") {
                                            currentState.gpsSats = frame.total_sats;
                                        } else if (gnssTypeStr == "GL") {
                                            currentState.glonassSats = frame.total_sats;
                                        } else if (gnssTypeStr == "GA") {
                                            currentState.galileoSats = frame.total_sats;
                                        } else if (gnssTypeStr == "GB") {
                                            currentState.beidouSats = frame.total_sats;
                                        } else if (gnssTypeStr == "GQ") {
                                            currentState.qzssSats = frame.total_sats;
                                        } else {
                                            spdlog::error("Unknown GNSS type: {}", gnssType);
                                        }
                                    }
                                } else {
                                    spdlog::error("Failed to parse GSV sentence: {}", nmeaBuffer);
                                }
                            }
                            break;

                            default:
                                spdlog::error("Unknown sentence: {}", nmeaBuffer);
                                break;
                        }
                    }
                }

                nmeaBufferCurrentIdx = 0;
                nmeaBufferStartValid = true;
            }

            nmeaBuffer[nmeaBufferCurrentIdx++] = c;
        }
    }

    if (!updates.empty()) {
        unprocessedUpdates = updates;
    }
}

std::optional<GpsUpdateList> GPS::getUnprocessedUpdates() {
    if (unprocessedUpdates.has_value()) {
        GpsUpdateList updates = unprocessedUpdates.value();
        unprocessedUpdates = std::nullopt;

        return updates;
    }

    return std::nullopt;
}

std::string GPS::removeNewLineCharacter(char *arr) {
    auto nmeaStr = std::string(arr);
    nmeaStr.erase(nmeaStr.size() - 1);

    return nmeaStr;
}
