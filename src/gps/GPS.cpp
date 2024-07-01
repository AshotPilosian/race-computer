#include <wiringx.h>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <span>

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

void GPS::setup() {
    spdlog::info("Setting up GPS");

    spdlog::debug("Opening GPS UART with baud 9600");
    uartFd = openUartToGpsModule(9600);
    spdlog::debug("Configure GPS module to increase baud to 115200");
    writeCommandToModule(UBX_CFG_UART1_BAUDRATE_115200);

    spdlog::debug("Closing GPS UART to re-open with higher rate");
    wiringXSerialClose(uartFd);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    spdlog::debug("Opening GPS UART with baud 115200");
    uartFd = openUartToGpsModule(115200);

    spdlog::debug("Sending GPS command: SIGNAL_QZSS_ENA = FALSE");
    writeCommandToModule(UBX_CFG_SIGNAL_QZSS_ENA_FALSE);

    spdlog::debug("Sending GPS command: SIGNAL_GLO_ENA = TRUE");
    writeCommandToModule(UBX_CFG_SIGNAL_GLO_ENA_TRUE);

    spdlog::debug("Sending GPS command: SIGNAL_GLO_L1_ENA = TRUE");
    writeCommandToModule(UBX_CFG_SIGNAL_GLO_L1_ENA_TRUE);

    spdlog::debug("Sending GPS command: NAVSPG_DYNMODEL = AUTOMOTIVE");
    writeCommandToModule(UBX_CFG_NAVSPG_DYNMODEL_AUTOMOTIVE);

    spdlog::debug("Sending GPS command: ODO_PROFILE = CAR");
    writeCommandToModule(UBX_CFG_ODO_PROFILE_CAR);

    spdlog::debug("Sending GPS command: HW_RF_LNA_MODE = NORMAL");
    writeCommandToModule(UBX_CFG_HW_RF_LNA_MODE_NORMAL);

    // spdlog::debug("Sending GPS command: NMEA_HIGHPREC = TRUE");
    // writeCommandToModule(UBX_CFG_NMEA_HIGHPREC_TRUE);

    spdlog::debug("Sending GPS command: ODO_USE_ODO = TRUE");
    writeCommandToModule(UBX_CFG_ODO_USE_ODO_TRUE);

    // spdlog::debug("Sending GPS command: RST = HOT");
    // writeCommandToModule(UBX_CFG_RST_HOT);

    // spdlog::debug("Sending GPS command: RATE_MEAS = 10HZ");
    // writeCommandToModule(UBX_CFG_RATE_MEAS_10HZ);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    spdlog::info("GPS UART setup completed. FD: {}", uartFd);
}

void GPS::shutdown() const {
    spdlog::info("Closing GPS connection");

    wiringXSerialClose(uartFd);

    spdlog::info("GPS connection closed");
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
                        handleValidNmeaSentence(updates);
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

// Private
void GPS::handleValidNmeaSentence(GpsUpdateList &gpsUpdates) {
    auto nmeaWithoutNewLineChar = removeNewLineCharacter(nmeaBuffer);
    spdlog::debug("NMEA: {}", nmeaWithoutNewLineChar);

    switch (minmea_sentence_id(nmeaBuffer, false)) {
        case MINMEA_SENTENCE_VTG:
            processVTG(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        case MINMEA_SENTENCE_GSA:
            processGSA(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        case MINMEA_SENTENCE_GLL:
            processGLL(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        case MINMEA_SENTENCE_RMC:
            processRMC(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        case MINMEA_SENTENCE_GGA:
            processGGA(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        case MINMEA_SENTENCE_GSV:
            processGSV(nmeaBuffer, nmeaWithoutNewLineChar, currentState, gpsUpdates);
            break;
        default:
            spdlog::error("Unknown sentence: {}", nmeaBuffer);
            break;
    }
}

void GPS::processGGA(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_gga frame{};
    if (minmea_parse_gga(&frame, buffer)) {
        stateToUpdate.hasFix = frame.fix_quality > 0;
        stateToUpdate.fixQuality = frame.fix_quality;
        stateToUpdate.numberOfSatellites = frame.satellites_tracked;
        stateToUpdate.hdop = minmea_tofloat(&frame.hdop);

        stateToUpdate.latitude = minmea_tocoord(&frame.latitude);
        stateToUpdate.longitude = minmea_tocoord(&frame.longitude);

        stateToUpdate.hours = frame.time.hours;
        stateToUpdate.minutes = frame.time.minutes;
        stateToUpdate.seconds = frame.time.seconds;
        stateToUpdate.microseconds = frame.time.microseconds;

        gpsUpdates.emplace_back(PositionUpdate{
            nmeaWithoutNewLine,
            frame.fix_quality > 0,
            minmea_tocoord(&frame.latitude),
            minmea_tocoord(&frame.longitude)
        });
    } else {
        spdlog::error("Failed to parse GGA sentence: {}", buffer);
    }
}

void GPS::processGSV(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_gsv frame{};
    if (minmea_parse_gsv(&frame, buffer)) {
        if (frame.msg_nr == 1) {
            char gnssType[3];
            strncpy(gnssType, buffer + 1, 2);
            gnssType[2] = '\0';

            if (const auto gnssTypeStr = std::string(gnssType); gnssTypeStr == "GP") {
                stateToUpdate.gpsSats = frame.total_sats;
            } else if (gnssTypeStr == "GL") {
                stateToUpdate.glonassSats = frame.total_sats;
            } else if (gnssTypeStr == "GA") {
                stateToUpdate.galileoSats = frame.total_sats;
            } else if (gnssTypeStr == "GB") {
                stateToUpdate.beidouSats = frame.total_sats;
            } else if (gnssTypeStr == "GQ") {
                stateToUpdate.qzssSats = frame.total_sats;
            } else {
                spdlog::error("Unknown GNSS type: {}", gnssType);
            }
        }
    } else {
        spdlog::error("Failed to parse GSV sentence: {}", buffer);
    }
}

void GPS::processRMC(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_rmc frame{};
    if (minmea_parse_rmc(&frame, buffer)) {
        stateToUpdate.hasFix = frame.valid;

        stateToUpdate.latitude = minmea_tocoord(&frame.latitude);
        stateToUpdate.longitude = minmea_tocoord(&frame.longitude);

        stateToUpdate.day = frame.date.day;
        stateToUpdate.month = frame.date.month;
        stateToUpdate.year = frame.date.year;

        stateToUpdate.hours = frame.time.hours;
        stateToUpdate.minutes = frame.time.minutes;
        stateToUpdate.seconds = frame.time.seconds;
        stateToUpdate.microseconds = frame.time.microseconds;

        // This speed is in knots, should be converted at first
        // currentState.speed = minmea_tofloat(&frame.speed);

        gpsUpdates.emplace_back(PositionUpdate{
            nmeaWithoutNewLine,
            frame.valid,
            minmea_tocoord(&frame.latitude),
            minmea_tocoord(&frame.longitude)
        });
    } else {
        spdlog::error("Failed to parse RMC sentence: {}", buffer);
    }
}

void GPS::processGLL(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_gll frame{};
    if (minmea_parse_gll(&frame, buffer)) {
        gpsUpdates.emplace_back(PositionUpdate{
            nmeaWithoutNewLine,
            false,
            minmea_tocoord(&frame.latitude),
            minmea_tocoord(&frame.longitude)
        });
    } else {
        spdlog::error("Failed to parse GLL sentence: {}", buffer);
    }
}

void GPS::processGSA(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_gsa frame{};
    if (minmea_parse_gsa(&frame, buffer)) {
        gpsUpdates.emplace_back(SatellitesUpdate{
            nmeaWithoutNewLine,
            0,
            minmea_tofloat(&frame.hdop)
        });
    } else {
        spdlog::error("Failed to parse GSA sentence: {}", buffer);
    }
}

void GPS::processVTG(const char *buffer,
                     const std::string &nmeaWithoutNewLine,
                     GpsState &stateToUpdate,
                     GpsUpdateList &gpsUpdates) {
    minmea_sentence_vtg frame{};
    if (minmea_parse_vtg(&frame, buffer)) {
        stateToUpdate.speed = minmea_tofloat(&frame.speed_kph);

        gpsUpdates.emplace_back(SpeedUpdate{
            nmeaWithoutNewLine,
            minmea_tofloat(&frame.speed_kph)
        });
    } else {
        spdlog::error("Failed to parse VTG sentence: {}", buffer);
    }
}

std::string GPS::removeNewLineCharacter(char *arr) {
    auto nmeaStr = std::string(arr);
    nmeaStr.erase(nmeaStr.size() - 1);

    return nmeaStr;
}

void GPS::writeCommandToModule(const std::span<const unsigned char> commandBytes) const {
    for (unsigned char ch: commandBytes) {
        spdlog::trace("Writing command char: {}", ch);
        wiringXSerialPutChar(uartFd, ch);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
