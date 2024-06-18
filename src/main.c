#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <wiringx.h>

#include "../lvgl/lvgl.h"
#include "../minmea/minmea.h"

#define TEXT_BUF_SIZE 84

typedef struct {
    lv_obj_t *label;
    char text[TEXT_BUF_SIZE];
} label_update_t;

// Function to handle LVGL tasks
void *lvgl_task_handler_thread(void *arg) {
    while (1) {
        lv_task_handler();
        usleep(10000); // Sleep for 10 milliseconds
    }
    return NULL;
}

uint8_t init_lvgl() {
    lv_init();

    lv_display_t *disp = lv_linux_fbdev_create();
    if (disp == NULL) {
        LV_LOG_ERROR("lv_demos initialization failure!");
        return 1;
    }
    lv_linux_fbdev_set_file(disp, "/dev/fb0");

    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

    // Create a thread for LVGL task handler
    pthread_t lvgl_thread;
    if (pthread_create(&lvgl_thread, NULL, lvgl_task_handler_thread, NULL) != 0) {
        perror("Failed to create LVGL task handler thread");
        return 1;
    }

    /* Create a screen with a yellow background */
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xffff00), LV_PART_MAIN);

    /* Create a custom style for the label */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_24); // Adjust the font size as needed

    /* Create a label and set its text */
    lv_obj_t *label = lv_label_create(scr);
    lv_obj_add_style(label, &style, LV_PART_MAIN);
    lv_label_set_text(label, "Hallo, \nich heisse Kartoffel");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    printf("LVGL setup completed\n");

    sleep(1);
    return 0;
}

int init_wiringX() {
    printf("Setting up wiringX\n");

    if (wiringXSetup("milkv_duo256m", NULL) == -1) {
        wiringXGC();
        return 1;
    }
    sleep(1);
    printf("wiringX setup completed\n");

    return 0;
}

int init_gps() {
    printf("Setting up GPS\n");

    struct wiringXSerial_t wiringXSerial = {9600, 8, 'n', 1, 'n'};
    int fd;

    if ((fd = wiringXSerialOpen("/dev/ttyS1", wiringXSerial)) < 0) {
        printf("Open serial device failed: %d\n", fd);
        wiringXGC();
        return -1;
    }
    sleep(1);
    printf("GPS UART setup completed. FD: %d\n", fd);

    return fd;
}

int close_gps(int fd) {
    printf("Closing GPS connection\n");

    wiringXSerialClose(fd);

    printf("GPS connection closed\n");
    return 0;
}

// Function to update the label text
void update_label_text(void *arg) {
    label_update_t *update_info = (label_update_t *) arg;
    lv_label_set_text(update_info->label, update_info->text);
    free(update_info); // Free the allocated memory for the struct
}

uint8_t calculate_checksum(const char *sentence) {
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

bool validate_checksum(const char *sentence, uint8_t calculated_checksum) {
    // Find the '*' character in the sentence
    const char *checksum_str = strchr(sentence, '*');
    if (checksum_str == NULL || strlen(checksum_str) < 3) {
        // '*' not found or not enough characters after '*' for checksum
        return false;
    }

    // Extract the checksum string and convert it to an integer
    uint8_t extracted_checksum = (uint8_t) strtol(checksum_str + 1, NULL, 16);

    // Compare the extracted checksum with the calculated checksum
    return calculated_checksum == extracted_checksum;
}

int main(int argc, char **argv) {
    printf("Setup started\n");

    system("duo-pinmux -w GP2/UART1_TX");
    system("duo-pinmux -w GP3/UART1_RX");
    sleep(1);

    init_wiringX();
    int fd = init_gps();

    printf("Setup finished\n");

    printf("Application started\n");

    char nmea_buffer[100];
    int available_bytes = 0, current_buffer_idx = 0, nmea_counter = 0;
    bool nmea_start_valid = false;
    while (nmea_counter < 1000) {
        available_bytes = wiringXSerialDataAvail(fd);
        if (available_bytes > 0) {
            //printf("Received bytes: %d\n", available_bytes);

            while (available_bytes--) {
                // printf("Reading char: ");
                char c = wiringXSerialGetChar(fd);
                // printf("%c; ", c);

                if (c == '$') {
                    // printf("Nmea start detected\n");
                    nmea_buffer[current_buffer_idx] = '\0';
                    if (nmea_start_valid) {
                        uint8_t calculated_checksum = calculate_checksum(nmea_buffer);
                        bool is_valid = validate_checksum(nmea_buffer, calculated_checksum);

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
                    //memset(nmea_buffer, 0, sizeof(nmea_buffer));
                }

                nmea_buffer[current_buffer_idx++] = c;
            }
        }
        nmea_counter = 10;
    }

    close_gps(fd);

    printf("Application closed\n");
}

// int main(int argc, char ** argv) {
//     printf("HALLLOOOOOOO\n");

//     // if (init_lvgl() != 0) {
//     //     return 0; 
//     // }
//     int fd = init_gps(); 


//     // Buffer to store serial data
//     // char buffer[84];

//     // while (1) {
//     //     length = wiringXSerialDataAvail(fd);
//     //     if (length > 0) {
//     //         i = 0;
//     //         while (length--) {
//     //             buffer[i++] = wiringXSerialGetChar(fd);
//     //         }
//     //         printf("Duo UART receive: %s\n", buffer);
//     //     }

//     //     if (length > 0) {
//     //         buffer[length] = '\0';
//     //         // Process the buffer for GPS data
//     //         switch (minmea_sentence_id(buffer, false)) {
//     //             case MINMEA_SENTENCE_RMC: {
//     //                 struct minmea_sentence_rmc frame;
//     //                 if (minmea_parse_rmc(&frame, buffer)) {
//     //                     printf("Latitude: %f, Longitude: %f\n",
//     //                            minmea_tocoord(&frame.latitude),
//     //                            minmea_tocoord(&frame.longitude));
//     //                 } else {
//     //                     printf("Failed to parse RMC sentence\n");
//     //                 }
//     //             } break;
//     //             case MINMEA_SENTENCE_GGA: {
//     //                 struct minmea_sentence_gga frame;
//     //                 if (minmea_parse_gga(&frame, buffer)) {
//     //                     printf("Latitude: %f, Longitude: %f, Altitude: %f\n",
//     //                            minmea_tocoord(&frame.latitude),
//     //                            minmea_tocoord(&frame.longitude),
//     //                            minmea_tofloat(&frame.altitude));
//     //                 } else {
//     //                     printf("Failed to parse GGA sentence\n");
//     //                 }
//     //             } break;
//     //             default:
//     //                 printf("Unknown sentence: %s\n", buffer);
//     //                 break;
//     //         }
//     //     } else if (length < 0) {
//     //         printf("Error reading from serial port: %s\n", strerror(errno));
//     //     }
//     // }

//     char buf[84];
//     int str_len, length = 0;
//     int i;

//     printf("Re-setting buffer\n");
//     //memset(buf, 0, sizeof(buf));
//     bool nmea_start_valid = false;
//     int nmea_counter = 0;
//     while(nmea_counter < 10)
//     {
//         str_len = wiringXSerialDataAvail(fd);
//         if (str_len > 0) {
//             printf("Some chars received: %d\n", str_len);

//             while (str_len--) {
//                 printf("Reading char; ");
//                 char c = wiringXSerialGetChar(fd);

//             //     if (c == '$') {
//             //         printf("Nmea start detected\n.");
//             //         if (nmea_start_valid) {
//             //             // Buffer now contains NMEA sentence, print it
//             //             uint8_t calculated_checksum = calculate_checksum(buf);
//             //             bool is_valid = validate_checksum(buf, calculated_checksum);

//             //             // printf("NMEA: %s", buf);
//             //             // printf("Checksum: %02X; Valid: %s\n\n", calculated_checksum, is_valid ? "TRUE" : "FALSE");

//             //             if (is_valid) {
//             //                 printf("NMEA: %s", buf);

//             //                 // label_update_t *update_info = (label_update_t *) malloc(sizeof(label_update_t));
//             //                 // update_info->label = label;

//             //                 // snprintf(update_info->text, TEXT_BUF_SIZE, "%s", buf); // Create the string with the counter
//             //                 // // printf("%s\n", update_info->text);

//             //                 // lv_async_call(update_label_text, update_info);
//             //             }

//             //             nmea_counter++;
//             //         }

//             //         nmea_start_valid = true;
//             //         i = 0;
//             //         memset(buf, 0, sizeof(buf));
//             //     }

//             //     buf[i++] = c;
//             }
//             //printf("Duo UART receive: %s\n", buf);
//         }
//     }

//     wiringXSerialClose(fd);

// /////////////////////////////////////////////

//     // for (int i = 0; i < 3; i++) {
//     //     label_update_t *update_info = (label_update_t *) malloc(sizeof(label_update_t));
//     //     update_info->label = label;

//     //     snprintf(update_info->text, TEXT_BUF_SIZE, "Updated: %d", i); // Create the string with the counter
//     //     printf("%s\n", update_info->text);

//     //     lv_async_call(update_label_text, update_info);

//     //     sleep(1);
//     // }

//     printf("CHAAAOOOO\n");
//     return 0;
// }

