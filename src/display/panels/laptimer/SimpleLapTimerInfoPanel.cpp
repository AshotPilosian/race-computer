#include "SimpleLapTimerInfoPanel.h"

#include <iomanip>
#include <sstream>

#include "spdlog/spdlog.h"

#include "../../font/Fonts.h"

SimpleLapTimerInfoPanel::SimpleLapTimerInfoPanel(lv_obj_t *parent): parent(parent) {
    initStyles();
    init();
}

void SimpleLapTimerInfoPanel::update(const LapTimerUpdateData *data) const {
    spdlog::trace("SimpleLapTimerInfoPanel::update -- Started");

    std::ostringstream oss;

    oss << "Lap: " << std::setw(2) << std::setfill(' ') << data->lapCounter;
    lv_label_set_text(widget.lapCounterLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Distance: " << std::fixed << std::setprecision(6) << data->distance;
    lv_label_set_text(widget.distanceLabel, oss.str().c_str());

    spdlog::trace("SimpleLapTimerInfoPanel::update -- Finished");
}

// Private
void SimpleLapTimerInfoPanel::initStyles() {
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

void SimpleLapTimerInfoPanel::init() {
    widget.container = lv_obj_create(parent);
    lv_obj_add_style(widget.container, &containerStyle, LV_PART_MAIN);
    lv_obj_set_size(widget.container, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(widget.container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(widget.container, LV_SCROLLBAR_MODE_OFF);

    widget.lapCounterLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.lapCounterLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.lapCounterLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.lapCounterLabel, "Lap: ");

    widget.distanceLabel = lv_label_create(widget.container);
    lv_obj_set_size(widget.distanceLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(widget.distanceLabel, &textStyle, LV_PART_MAIN);
    lv_label_set_text(widget.distanceLabel, "Distance: ");
}
