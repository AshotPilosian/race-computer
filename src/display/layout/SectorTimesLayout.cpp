#include "SectorTimesLayout.h"

#include "../Colors.h"
#include "spdlog/spdlog.h"

SectorTimesLayout::SectorTimesLayout(Display *display): display(display),
                                                        mainContainer(nullptr),
                                                        sectorsPanel(nullptr),
                                                        gpsInfoPanel(nullptr),
                                                        lapTimerPanel(nullptr) {
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
    init();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    spdlog::info("SectorTimesLayout setup completed");
}

void SectorTimesLayout::update(const SectorTimeLayoutUpdateData *updateData) const {
    spdlog::trace("SectorTimesLayout::update -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new SectorTimeLayoutUpdateInternalData{
        new ColoredSectorTimesPanelUpdateData(
            sectorsPanel,
            new SectorTimesUpdateData(updateData->sector1, updateData->sector2, updateData->sector3)
        ),
        new GpsPositionExtendedInfoPanelUpdateData(gpsInfoPanel, &(updateData->gpsInfo)),
        new SimpleLapTimerInfoPanelUpdateData(lapTimerPanel, &(updateData->lapTimerInfo))
    };

    display->lvgl_mutex.lock();
    lv_async_call(doUpdate, data);
    display->lvgl_mutex.unlock();

    spdlog::trace("SectorTimesLayout::update -- Update scheduled");
}

// Private
void SectorTimesLayout::initStyles() {
    lv_style_init(&mainContainerStyle);
    lv_style_set_border_width(&mainContainerStyle, 0);
    lv_style_set_radius(&mainContainerStyle, 0);
    lv_style_set_pad_all(&mainContainerStyle, 0);
    lv_style_set_pad_column(&mainContainerStyle, 0);
    lv_style_set_pad_row(&mainContainerStyle, 0);
}

void SectorTimesLayout::init() {
    mainContainer = lv_obj_create(lv_screen_active());
    lv_obj_add_style(mainContainer, &mainContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(mainContainer, lv_pct(100), lv_pct(100));
    lv_obj_set_style_bg_color(mainContainer, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_scrollbar_mode(mainContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(mainContainer, LV_FLEX_FLOW_COLUMN);

    sectorsPanel = new ColoredSectorTimesPanel(mainContainer, lv_pct(15));
    gpsInfoPanel = new GpsPositionExtendedInfoPanel(mainContainer);
    lapTimerPanel = new SimpleLapTimerInfoPanel(mainContainer);
}

void SectorTimesLayout::doUpdate(void *param) {
    const auto *data = static_cast<SectorTimeLayoutUpdateInternalData *>(param);
    if (data) {
        data->sectors->widget->update(data->sectors->data);
        data->gpsInfo->widget->update(data->gpsInfo->data);
        data->lapTimerInfo->widget->update(data->lapTimerInfo->data);
    }

    delete data;
}
