#include <iostream>
#include <unistd.h>

#include "lvgl.h"

#include "Display.h"

#include "spdlog/spdlog.h"

Display::Display(): running(false) {
}

auto Display::lvglMainLoop() -> void {
    while (running) {
        lv_timer_handler();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

SectorTimeWidget Display::createSectorTimeWidget(lv_obj_t *parent, const int32_t width, const int32_t offsetX) const {
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_add_style(container, &sectorTimeContainerStyle, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(container, width, lv_pct(20));
    lv_obj_set_pos(container, offsetX, 0);
    lv_obj_set_style_bg_color(container, colorGrey, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(container);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(label, &sectorTimeTextStyle, LV_PART_MAIN);
    lv_label_set_text(label, "");

    return SectorTimeWidget{container, label};
}

void Display::updateSectorTimeWidget(SectorTimeWidgetData &sectorTimeWidgetData) {
    spdlog::trace("Display.updateSectorTimeWidget -- Started");

    lv_obj_set_style_bg_color(sectorTimeWidgetData.widget.container, sectorTimeWidgetData.color, LV_PART_MAIN);
    spdlog::trace("Display.updateSectorTimeWidget -- Set color");

    lv_label_set_text(sectorTimeWidgetData.widget.label, sectorTimeWidgetData.text.data());
    spdlog::trace("Display.updateSectorTimeWidget -- Set text: {}", sectorTimeWidgetData.text);

    spdlog::trace("Display.updateSectorTimeWidget -- Finished");
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
    initStyles();

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

    sleep(1);

    mainContainer = lv_obj_create(lv_screen_active()); /*Create a parent object on the current screen*/
    lv_obj_add_style(mainContainer, &mainContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(mainContainer, lv_pct(100), lv_pct(100)); /*Set the size of the parent*/
    lv_obj_set_style_bg_color(mainContainer, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(mainContainer, LV_SCROLLBAR_MODE_OFF);

    sectorWidget1 = createSectorTimeWidget(mainContainer, lv_pct(33) + 1, -1);
    sectorWidget2 = createSectorTimeWidget(mainContainer, lv_pct(35), lv_pct(33));
    sectorWidget3 = createSectorTimeWidget(mainContainer, lv_pct(34), lv_pct(33 + 34));

    spdlog::info("LVGL setup completed");
}

void Display::initStyles() {
    lv_style_init(&mainContainerStyle);
    lv_style_set_border_width(&mainContainerStyle, 0);
    lv_style_set_radius(&mainContainerStyle, 0);
    lv_style_set_pad_all(&mainContainerStyle, 0);

    lv_style_init(&sectorTimeTextStyle);
    lv_style_set_text_font(&sectorTimeTextStyle, &lv_font_montserrat_20);

    lv_style_init(&sectorTimeContainerStyle);
    lv_style_set_radius(&sectorTimeContainerStyle, 0);
    lv_style_set_pad_all(&sectorTimeContainerStyle, 0);
    lv_style_set_border_side(&sectorTimeContainerStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT);
    lv_style_set_border_width(&sectorTimeContainerStyle, 1);
}

void Display::updateSectorTimes(void *param) {
    auto *data = static_cast<LayoutUpdateData *>(param);
    if (data) {
        updateSectorTimeWidget(data->sector1);
        updateSectorTimeWidget(data->sector2);
        updateSectorTimeWidget(data->sector3);
    }

    delete data;
}

void Display::update() const {
    spdlog::trace("Display.updateSectorTimes -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new LayoutUpdateData{
        {sectorWidget1, colorFasterThanPrevious, "-0.13"},
        {sectorWidget2, colorFastestOverall, "-0.06"},
        {sectorWidget3, colorSlowerThanPrevious, "+0.47"}
    };

    lv_async_call(updateSectorTimes, data);

    spdlog::trace("Display.updateSectorTimes -- Update scheduled");
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
