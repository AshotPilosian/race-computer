#include <wiringx.h>
#include <optional>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <cstring>

#include "spdlog/spdlog.h"
#include "../minmea/minmea.h"

#include "GPS.h"

GPS::GPS() {
    nmeaBufferCurrentIdx = 0;
    nmeaBufferStartValid = false;
}

void GPS::setup() {
    spdlog::info("Setting up GPS");

    struct wiringXSerial_t wiringXSerial = {9600, 8, 'n', 1, 'n'};
    int fd;

    if ((fd = wiringXSerialOpen("/dev/ttyS1", wiringXSerial)) < 0) {
        spdlog::critical("Open serial device failed: {}", fd);
        wiringXGC();

        throw std::runtime_error("Can not open GPS UART.");
    }
    uartFd = fd;

    spdlog::info("GPS UART setup completed. FD: {}", fd);
}

void GPS::shutdown() {
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

std::optional <uint8_t> GPS::extractChecksum(const char *sentence) {
    // Find the '*' character in the sentence
    const char *checksumStr = strchr(sentence, '*');
    if (checksumStr == NULL || strlen(checksumStr) < 3) {
        // '*' not found or not enough characters after '*' for checksum
        return std::nullopt;
    }

    // Extract the checksum string and convert it to an integer
    return (uint8_t) strtol(checksumStr + 1, NULL, 16);
}

bool GPS::validateChecksum(const char *sentence) {
    std::optional <uint8_t> extractedChecksum = extractChecksum(sentence);
    if (!extractedChecksum.has_value()) {
        return false;
    }

    uint8_t calculatedChecksum = calculateChecksum(sentence);

    return calculatedChecksum == extractedChecksum.value();
}

void GPS::readAvailable() {
    int availableBytes = wiringXSerialDataAvail(uartFd);
    if (availableBytes > 0) {
        spdlog::trace("Received bytes: {}", availableBytes);
        while (availableBytes--) {
            char c = wiringXSerialGetChar(uartFd);
            spdlog::trace("Reading char: {}", c);

            if (c == '$') {
                spdlog::trace("Nmea start detected");
                nmeaBuffer[nmeaBufferCurrentIdx] = '\0';
                if (nmeaBufferStartValid) {
                    bool validChecksum = validateChecksum(nmeaBuffer);
                    if (validChecksum) {
                        spdlog::trace("NMEA: {}", nmeaBuffer);
                        unprocessedUpdate = PositionUpdate{nmeaBuffer, 0.0, 0.0};

                        switch (minmea_sentence_id(nmeaBuffer, false)) {
                            case MINMEA_SENTENCE_VTG: {
                                struct minmea_sentence_vtg frame;
                                if (minmea_parse_vtg(&frame, nmeaBuffer)) {
                                    unprocessedUpdate = SpeedUpdate{nmeaBuffer, minmea_tofloat(&frame.speed_kph)};
                                } else {
                                    spdlog::error("Failed to parse VTG sentence: {}", nmeaBuffer);
                                }
                            }
                                break;
                            case MINMEA_SENTENCE_GSA: {
                                struct minmea_sentence_gsa frame;
                                if (minmea_parse_gsa(&frame, nmeaBuffer)) {
                                    unprocessedUpdate = SatellitesUpdate{nmeaBuffer, 0, minmea_tofloat(&frame.hdop)};
                                } else {
                                    spdlog::error("Failed to parse GSA sentence: {}", nmeaBuffer);
                                }
                            }
                                break;
                            case MINMEA_SENTENCE_GLL: {
                                struct minmea_sentence_gll frame;
                                if (minmea_parse_gll(&frame, nmeaBuffer)) {
                                    unprocessedUpdate = PositionUpdate{
                                            nmeaBuffer,
                                            minmea_tocoord(&frame.latitude),
                                            minmea_tocoord(&frame.longitude)
                                    };
                                } else {
                                    spdlog::error("Failed to parse GLL sentence: {}", nmeaBuffer);
                                }
                            }
                                break;
                            case MINMEA_SENTENCE_RMC: {
                                struct minmea_sentence_rmc frame;
                                if (minmea_parse_rmc(&frame, nmeaBuffer)) {
                                    unprocessedUpdate = PositionUpdate{
                                            nmeaBuffer,
                                            minmea_tocoord(&frame.latitude),
                                            minmea_tocoord(&frame.longitude)
                                    };
                                } else {
                                    spdlog::error("Failed to parse RMC sentence: {}", nmeaBuffer);
                                }
                            }
                                break;
                            case MINMEA_SENTENCE_GGA: {
                                struct minmea_sentence_gga frame;
                                if (minmea_parse_gga(&frame, nmeaBuffer)) {
                                    unprocessedUpdate = PositionUpdate{
                                            nmeaBuffer,
                                            minmea_tocoord(&frame.latitude),
                                            minmea_tocoord(&frame.longitude)
                                    };
                                } else {
                                    spdlog::error("Failed to parse GGA sentence: {}", nmeaBuffer);
                                }
                            }
                                break;
                            default:
                                spdlog::error("Unknown sentence: {}", nmeaBuffer);
                                break;
                        }

                        // label_update_t *update_info = (label_update_t *) malloc(sizeof(label_update_t));
                        // update_info->label = label;

                        // snprintf(update_info->text, TEXT_BUF_SIZE, "%s", buf); // Create the string with the counter
                        // printf("%s\n", update_info->text);

                        // lv_async_call(update_label_text, update_info);
                    }
                }

                nmeaBufferCurrentIdx = 0;
                nmeaBufferStartValid = true;
            }

            nmeaBuffer[nmeaBufferCurrentIdx++] = c;
        }
    }
}

std::optional <GpsUpdate> GPS::getUnprocessedUpdate() {
    if (unprocessedUpdate.has_value()) {
        GpsUpdate update = unprocessedUpdate.value();
        unprocessedUpdate = std::nullopt;

        return update;
    } else {
        return std::nullopt;
    }
}
