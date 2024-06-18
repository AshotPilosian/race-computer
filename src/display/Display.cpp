#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "lvgl.h"

#include "Display.h"

Display::Display() {

}

void *lvgl_task_handler_thread(void *arg) {
    while (1) {
        lv_task_handler();
        usleep(10000); // Sleep for 10 milliseconds
    }
    return NULL;
}

void Display::setup() {
    printf("Setting up LVGL\n");

    lv_init();

    lv_display_t *disp = lv_linux_fbdev_create();
    if (disp == NULL) {
        throw std::runtime_error("Can not create Linux Framebufer based LVGL display.");
    }
    lv_linux_fbdev_set_file(disp, "/dev/fb0");

    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

    // Create a thread for LVGL task handler
    pthread_t lvgl_thread;
    if (pthread_create(&lvgl_thread, NULL, lvgl_task_handler_thread, NULL) != 0) {
        throw std::runtime_error("Failed to create LVGL task handler thread");
    }

    /* Create a screen with a yellow background */
    lv_obj_t * scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xffff00), LV_PART_MAIN);

    /* Create a custom style for the label */
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_montserrat_24); // Adjust the font size as needed

    /* Create a label and set its text */
    lv_obj_t * label = lv_label_create(scr);
    lv_obj_add_style(label, &style, LV_PART_MAIN);
    lv_label_set_text(label, "Hallo, \nich heisse Kartoffel");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    printf("LVGL setup completed\n");
}

void Display::shutdown() {

}
