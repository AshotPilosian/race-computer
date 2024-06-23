#include "SectorTimesLayout.h"

#include "spdlog/spdlog.h"

lv_color_t SectorTimesLayout::toColor(const SectorState sectorState) {
    switch (sectorState) {
        case NOT_STARTED:
            return colorGrey;
            break;
        case IN_PROGRESS:
            return colorLightGrey;
            break;
        case SLOWER_THEN_PREVIOUS_LAP:
            return colorSlowerThanPrevious;
            break;
        case FASTER_THEN_PREVIOUS_LAP:
            return colorFasterThanPrevious;
            break;
        case BEST:
            return colorFastestOverall;
            break;
        default:
            throw std::invalid_argument("Unsupported sector type.");
    }
}

SectorTimesLayout::SectorTimesLayout(Display *_display): display(_display) {
}

void SectorTimesLayout::setup() {
    spdlog::info("Setting up SectorTimesLayout");

    if (!display->initialized) {
        spdlog::error("Display not initialized!");

        throw std::logic_error("Display should be initialized.");
    }

    initStyles();

    mainContainer = lv_obj_create(lv_screen_active()); /*Create a parent object on the current screen*/
    lv_obj_add_style(mainContainer, &mainContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(mainContainer, lv_pct(100), lv_pct(100)); /*Set the size of the parent*/
    lv_obj_set_style_bg_color(mainContainer, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(mainContainer, LV_SCROLLBAR_MODE_OFF);

    sectorWidget1 = createSectorTimeWidget(mainContainer, lv_pct(33) + 1, -1);
    sectorWidget2 = createSectorTimeWidget(mainContainer, lv_pct(35), lv_pct(33));
    sectorWidget3 = createSectorTimeWidget(mainContainer, lv_pct(34), lv_pct(33 + 34));

    spdlog::info("SectorTimesLayout setup completed");
}

void SectorTimesLayout::initStyles() {
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

void SectorTimesLayout::updateSectorTimes(void *param) {
    auto *data = static_cast<SectorTimeLayoutUpdateInternalData *>(param);
    if (data) {
        updateSectorTimeWidget(data->sector1);
        updateSectorTimeWidget(data->sector2);
        updateSectorTimeWidget(data->sector3);
    }

    delete data;
}

void SectorTimesLayout::update(const SectorTimeLayoutUpdateData &updateData) const {
    spdlog::trace("Display.updateSectorTimes -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new SectorTimeLayoutUpdateInternalData{
        {sectorWidget1, toColor(updateData.sector1.state), updateData.sector1.time},
        {sectorWidget2, toColor(updateData.sector2.state), updateData.sector2.time},
        {sectorWidget3, toColor(updateData.sector3.state), updateData.sector3.time}
    };

    lv_async_call(updateSectorTimes, data);

    spdlog::trace("Display.updateSectorTimes -- Update scheduled");
}

SectorTimeWidget SectorTimesLayout::createSectorTimeWidget(lv_obj_t *parent, const int32_t width,
                                                           const int32_t offsetX) const {
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

void SectorTimesLayout::updateSectorTimeWidget(SectorTimeWidgetData &sectorTimeWidgetData) {
    spdlog::trace("Display.updateSectorTimeWidget -- Started");

    lv_obj_set_style_bg_color(sectorTimeWidgetData.widget.container, sectorTimeWidgetData.color, LV_PART_MAIN);
    spdlog::trace("Display.updateSectorTimeWidget -- Set color");

    lv_label_set_text(sectorTimeWidgetData.widget.label, sectorTimeWidgetData.text.data());
    spdlog::trace("Display.updateSectorTimeWidget -- Set text: {}", sectorTimeWidgetData.text);

    spdlog::trace("Display.updateSectorTimeWidget -- Finished");
}
