#include <wiringx.h>
#include <optional>
#include <cstdio>
#include <stdexcept>
#include <cstdint>
#include <cstring>

#include "../minmea/minmea.h"

#include "GPS.h"

GPS::GPS() {

}

void GPS::setup() {
    printf("Setting up GPS\n");

    struct wiringXSerial_t wiringXSerial = {9600, 8, 'n', 1, 'n'};
    int fd;

    if ((fd = wiringXSerialOpen("/dev/ttyS1", wiringXSerial)) < 0) {
        printf("Open serial device failed: %d\n", fd);
        wiringXGC();

        throw std::runtime_error("Can not open GPS UART.");
    }
    uartFd = fd;

    printf("GPS UART setup completed. FD: %d\n", fd);
}

void GPS::shutdown() {
    printf("Closing GPS connection\n");

    wiringXSerialClose(uartFd);

    printf("GPS connection closed\n");
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

void GPS::start() {
    char nmea_buffer[100];
    int available_bytes = 0, current_buffer_idx = 0, nmea_counter = 0;
    bool nmea_start_valid = false;
    while (nmea_counter < 1000) {
        available_bytes = wiringXSerialDataAvail(uartFd);
        if (available_bytes > 0) {
            //printf("Received bytes: %d\n", available_bytes);

            while (available_bytes--) {
                // printf("Reading char: ");
                char c = wiringXSerialGetChar(uartFd);
                // printf("%c; ", c);

                if (c == '$') {
                    // printf("Nmea start detected\n");
                    nmea_buffer[current_buffer_idx] = '\0';
                    if (nmea_start_valid) {
                        bool is_valid = validateChecksum(nmea_buffer);

                        // printf("NMEA: %s", nmea_buffer);
                        // printf("Checksum: %02X; Valid: %s\n\n", calculated_checksum, is_valid ? "TRUE" : "FALSE");

                        if (is_valid) {
                            // printf("NMEA: %s", nmea_buffer);

                            switch (minmea_sentence_id(nmea_buffer, false)) {
                                case MINMEA_SENTENCE_VTG: {
                                    struct minmea_sentence_vtg frame;
                                    if (minmea_parse_vtg(&frame, nmea_buffer)) {
                                        printf("Speed: %f\n",
                                               minmea_tofloat(&frame.speed_kph));
                                    } else {
                                        printf("Failed to parse VTG sentence\n");
                                    }
                                }
                                    break;
                                case MINMEA_SENTENCE_GSA: {
                                    struct minmea_sentence_gsa frame;
                                    if (minmea_parse_gsa(&frame, nmea_buffer)) {
                                        printf("HDOP: %f\n",
                                               minmea_tofloat(&frame.hdop));
                                    } else {
                                        printf("Failed to parse GSA sentence\n");
                                    }
                                }
                                    break;
                                case MINMEA_SENTENCE_GLL: {
                                    struct minmea_sentence_gll frame;
                                    if (minmea_parse_gll(&frame, nmea_buffer)) {
                                        printf("Latitude: %f, Longitude: %f\n",
                                               minmea_tocoord(&frame.latitude),
                                               minmea_tocoord(&frame.longitude));
                                    } else {
                                        printf("Failed to parse GLL sentence\n");
                                    }
                                }
                                    break;
                                case MINMEA_SENTENCE_RMC: {
                                    struct minmea_sentence_rmc frame;
                                    if (minmea_parse_rmc(&frame, nmea_buffer)) {
                                        printf("Latitude: %f, Longitude: %f\n",
                                               minmea_tocoord(&frame.latitude),
                                               minmea_tocoord(&frame.longitude));
                                    } else {
                                        printf("Failed to parse RMC sentence\n");
                                    }
                                }
                                    break;
                                case MINMEA_SENTENCE_GGA: {
                                    struct minmea_sentence_gga frame;
                                    if (minmea_parse_gga(&frame, nmea_buffer)) {
                                        printf("Latitude: %f, Longitude: %f, Altitude: %f\n",
                                               minmea_tocoord(&frame.latitude),
                                               minmea_tocoord(&frame.longitude),
                                               minmea_tofloat(&frame.altitude));
                                    } else {
                                        printf("Failed to parse GGA sentence\n");
                                    }
                                }
                                    break;
                                default:
                                    printf("Unknown sentence: %s\n", nmea_buffer);
                                    break;
                            }

                            // label_update_t *update_info = (label_update_t *) malloc(sizeof(label_update_t));
                            // update_info->label = label;

                            // snprintf(update_info->text, TEXT_BUF_SIZE, "%s", buf); // Create the string with the counter
                            // printf("%s\n", update_info->text);

                            // lv_async_call(update_label_text, update_info);
                        }

                        nmea_counter++;
                    }

                    nmea_start_valid = true;
                    current_buffer_idx = 0;
                }

                nmea_buffer[current_buffer_idx++] = c;
            }
        }
    }
}