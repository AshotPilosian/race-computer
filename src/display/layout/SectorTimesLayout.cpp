#include "SectorTimesLayout.h"

#include "Colors.h"
#include "spdlog/spdlog.h"

SectorTimesLayout::SectorTimesLayout(Display *_display): display(_display),
                                                         sectorsPanel(nullptr),
                                                         gpsInfoPanel(nullptr) {
}

SectorTimesLayout::~SectorTimesLayout() {
    delete sectorsPanel;
    delete gpsInfoPanel;
    delete lapTimerPanel;
}

void SectorTimesLayout::setup() {
    spdlog::info("Setting up SectorTimesLayout");

    if (!display->initialized) {
        spdlog::error("Display not initialized!");

        throw std::logic_error("Display should be initialized.");
    }

    initStyles();

    mainContainer = lv_obj_create(lv_screen_active());
    lv_obj_add_style(mainContainer, &mainContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(mainContainer, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(mainContainer, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(mainContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(mainContainer, LV_FLEX_FLOW_COLUMN);

    sectorsPanel = new ColoredSectorTimesPanel(mainContainer, lv_pct(15));
    gpsInfoPanel = new GpsPositionExtendedInfoPanel(mainContainer);
    lapTimerPanel = new SimpleLapTimerInfoPanel(mainContainer);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    spdlog::info("SectorTimesLayout setup completed");
}

void SectorTimesLayout::initStyles() {
    LV_FONT_DECLARE(martian_mono_16);

    lv_style_init(&mainContainerStyle);
    lv_style_set_border_width(&mainContainerStyle, 0);
    lv_style_set_radius(&mainContainerStyle, 0);
    lv_style_set_pad_all(&mainContainerStyle, 0);
    lv_style_set_pad_column(&mainContainerStyle, 0);
    lv_style_set_pad_row(&mainContainerStyle, 0);
}

void SectorTimesLayout::updateSectorTimes(void *param) {
    auto *data = static_cast<SectorTimeLayoutUpdateInternalData *>(param);
    if (data) {
        data->sectors.widget->update(data->sectors.data);
        data->gpsInfo.widget->update(data->gpsInfo.data);
        data->lapTimerInfo.widget->update(data->lapTimerInfo.data);
    }

    delete data;
}

void SectorTimesLayout::update(const SectorTimeLayoutUpdateData &updateData) const {
    spdlog::trace("Display.updateSectorTimes -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new SectorTimeLayoutUpdateInternalData{
        {sectorsPanel, {updateData.sector1, updateData.sector2, updateData.sector3}},
        {gpsInfoPanel, updateData.gpsInfo},
        {lapTimerPanel, updateData.lapTimerInfo}
    };

    lv_async_call(updateSectorTimes, data);

    spdlog::trace("Display.updateSectorTimes -- Update scheduled");
}
