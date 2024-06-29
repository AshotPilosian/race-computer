#include "SectorTimeColoredWidget.h"

#include "spdlog/spdlog.h"

#include "../../font/Fonts.h"
#include "../../Colors.h"

SectorTimeColoredWidget::SectorTimeColoredWidget(lv_obj_t *parent, const int32_t width): parent(parent), width(width) {
    initStyles();
    init();
}

void SectorTimeColoredWidget::update(const SectorTimeUpdateData &sectorTimeUpdateData) const {
    spdlog::trace("SectorTimeColoredWidget.update -- Started");

    lv_obj_set_style_bg_color(widget.container, toColor(sectorTimeUpdateData.state), LV_PART_MAIN);
    spdlog::trace("SectorTimeColoredWidget.update -- Set color");

    lv_label_set_text(widget.label, sectorTimeUpdateData.time.data());
    spdlog::trace("SectorTimeColoredWidget.update -- Set text: {}", sectorTimeUpdateData.time);

    spdlog::trace("SectorTimeColoredWidget.update -- Finished");
}

// Private
void SectorTimeColoredWidget::initStyles() {
    lv_style_init(&textStyle);
    lv_style_set_text_font(&textStyle, &martian_mono_20);

    lv_style_init(&containerStyle);
    lv_style_set_radius(&containerStyle, 0);
    lv_style_set_pad_all(&containerStyle, 0);
    lv_style_set_border_width(&containerStyle, 0);
}

void SectorTimeColoredWidget::init() {
    lv_obj_t *container = lv_obj_create(parent);
    lv_obj_add_style(container, &containerStyle, LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_size(container, width, lv_pct(100));
    lv_obj_set_style_bg_color(container, colorGrey, LV_PART_MAIN);

    lv_obj_t *label = lv_label_create(container);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(label, &textStyle, LV_PART_MAIN);
    lv_label_set_text(label, "");

    widget.container = container;
    widget.label = label;
}

lv_color_t SectorTimeColoredWidget::toColor(const SectorState sectorState) {
    switch (sectorState) {
        case NOT_STARTED:
            return colorGrey;
        case IN_PROGRESS:
            return colorLightGrey;
        case SLOWER_THEN_PREVIOUS_LAP:
            return colorSlowerThanPrevious;
        case FASTER_THEN_PREVIOUS_LAP:
            return colorFasterThanPrevious;
        case BEST:
            return colorFastestOverall;
        default:
            throw std::invalid_argument("Unsupported sector type.");
    }
}
