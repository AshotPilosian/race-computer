#include "GpsPositionExtendedInfoPanel.h"

#include <iomanip>
#include <sstream>
#include <string>

#include "spdlog/spdlog.h"

GpsPositionExtendedInfoPanel::GpsPositionExtendedInfoPanel(lv_obj_t *parent): parent(parent) {
    initStyles();
    init();
}

void GpsPositionExtendedInfoPanel::update(const GpsState &data) const {
    spdlog::trace("GpsPositionExtendedInfoPanel.update -- Started");

    std::ostringstream oss;

    oss << "Fix: " << (data.hasFix ? 1 : 0) << "  "
            << "Sat: " << std::setw(2) << std::setfill(' ') << data.numberOfSatellites << "  "
            << "Hdop: " << std::fixed << std::setprecision(2) << data.hdop;
    lv_label_set_text(widget.fixInfoLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "P:" << std::setw(2) << std::setfill(' ') << data.gpsSats << " "
            << "L:" << std::setw(2) << std::setfill(' ') << data.glonassSats << " "
            << "A:" << std::setw(2) << std::setfill(' ') << data.galileoSats << " "
            << "B:" << std::setw(2) << std::setfill(' ') << data.beidouSats << " "
            << "Q:" << std::setw(2) << std::setfill(' ') << data.qzssSats;
    lv_label_set_text(widget.satStatsLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Time:"
            // << " " << std::setw(2) << std::setfill('0') << data.day
            // << "." << std::setw(2) << std::setfill('0') << data.month
            // << "." << std::setw(4) << std::setfill('0') << data.year
            << " " << std::setw(2) << std::setfill('0') << data.hours
            << ":" << std::setw(2) << std::setfill('0') << data.minutes
            << ":" << std::setw(2) << std::setfill('0') << data.seconds
            << "." << std::setw(3) << std::setfill('0') << data.microseconds;
    lv_label_set_text(widget.timeLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Lat: " << std::fixed << std::setprecision(6) << data.latitude;
    lv_label_set_text(widget.latLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Lon: " << std::fixed << std::setprecision(6) << data.longitude;
    lv_label_set_text(widget.lonLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Speed: " << std::fixed << std::setprecision(2) << data.speed;
    lv_label_set_text(widget.speedLabel, oss.str().c_str());

    spdlog::trace("GpsPositionExtendedInfoPanel.update -- Finished");
}

// Private
void GpsPositionExtendedInfoPanel::initStyles() {
    LV_FONT_DECLARE(martian_mono_16);

    lv_style_init(&containerStyle);
    lv_style_set_radius(&containerStyle, 0);
    lv_style_set_pad_all(&containerStyle, 0);
    lv_style_set_pad_top(&containerStyle, 2);
    lv_style_set_pad_row(&containerStyle, 0);
    lv_style_set_pad_column(&containerStyle, 0);
    lv_style_set_border_side(&containerStyle, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&containerStyle, 1);

    lv_style_init(&textStyle);
    lv_style_set_text_font(&textStyle, &martian_mono_16);
}

void GpsPositionExtendedInfoPanel::init() {
    widget.container = lv_obj_create(parent);
    lv_obj_add_style(widget.container, &containerStyle, LV_PART_MAIN);
    lv_obj_set_size(widget.container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(widget.container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(widget.container, LV_SCROLLBAR_MODE_OFF);

    widget.satStatsLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.satStatsLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.satStatsLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.satStatsLabel, "GP: ");

    widget.fixInfoLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.fixInfoLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.fixInfoLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.fixInfoLabel, "Fix: ");

    widget.timeLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.timeLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.timeLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.timeLabel, "Time: ");

    widget.latLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.latLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.latLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.latLabel, "Lat: ");

    widget.lonLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.lonLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.lonLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.lonLabel, "Lon: ");

    widget.speedLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.speedLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.speedLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.speedLabel, "Speed: ");
}
