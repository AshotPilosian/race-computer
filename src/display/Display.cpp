#include <unistd.h>

#include "lvgl.h"

#include "Display.h"

#include "spdlog/spdlog.h"

Display::Display(): running(false), initialized(false) {
}

auto Display::lvglMainLoop() -> void {
    while (running) {
        lvgl_mutex.lock();
        lv_timer_handler();
        lvgl_mutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void Display::initLvgl() {
    lv_init();

    lv_display_t *disp = lv_linux_fbdev_create();
    if (disp == nullptr) {
        throw std::runtime_error("Can not create Linux Framebufer based LVGL display.");
    }
    lv_linux_fbdev_set_file(disp, "/dev/fb0");

    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

    running = true;
    lvglThread = std::thread(&Display::lvglMainLoop, this);
}

void Display::setup() {
    spdlog::info("Setting up LVGL");

    initLvgl();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    /* Create a screen with a yellow background */
    lv_obj_t *scr = lv_screen_active();
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

    std::this_thread::sleep_for(std::chrono::seconds(1));

    initialized = true;

    spdlog::info("LVGL setup completed");
}

void Display::resetScreenColor(void *arg) {
    lv_obj_t *scr = lv_screen_active();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), LV_PART_MAIN);
}

void Display::shutdown() {
    spdlog::info("Closing LVGL");

    lv_async_call(resetScreenColor, nullptr);
    sleep(1);

    if (running) {
        running = false;
        if (lvglThread.joinable()) {
            lvglThread.join();
        }
    }

    spdlog::info("LVGL closed");
}
