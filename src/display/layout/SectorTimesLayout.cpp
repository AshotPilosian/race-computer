#include "SectorTimesLayout.h"

#include <iomanip>
#include <sstream>

#include "Colors.h"
#include "spdlog/spdlog.h"

SectorTimesLayout::SectorTimesLayout(Display *_display): display(_display),
                                                         sectorWidget1(nullptr),
                                                         sectorWidget2(nullptr),
                                                         sectorWidget3(nullptr) {
}

SectorTimesLayout::~SectorTimesLayout() {
    delete sectorWidget1;
    delete sectorWidget2;
    delete sectorWidget3;
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

    lv_obj_t *sectorTimesContainer = lv_obj_create(mainContainer);
    lv_obj_add_style(sectorTimesContainer, &sectorTimesContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(sectorTimesContainer, lv_pct(100), lv_pct(15));
    lv_obj_set_pos(sectorTimesContainer, 0, 0);
    lv_obj_set_flex_flow(sectorTimesContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_scrollbar_mode(sectorTimesContainer, LV_SCROLLBAR_MODE_OFF);

    sectorWidget1 = new SectorTimeColoredWidget(sectorTimesContainer, lv_pct(33));
    sectorWidget2 = new SectorTimeColoredWidget(sectorTimesContainer, lv_pct(35));
    sectorWidget3 = new SectorTimeColoredWidget(sectorTimesContainer, lv_pct(33));

    gpsInfoWidget = createGpsInfoWidget(mainContainer);

    lapTimerInfoWidget = createLapTimerInfoWidget(mainContainer);

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

    lv_style_init(&sectorTimesContainerStyle);
    lv_style_set_radius(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_all(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_column(&sectorTimesContainerStyle, 0);
    lv_style_set_pad_row(&sectorTimesContainerStyle, 0);
    lv_style_set_border_side(&sectorTimesContainerStyle, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&sectorTimesContainerStyle, 1);

    lv_style_init(&gpsInfoContainerStyle);
    lv_style_set_radius(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_all(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_top(&gpsInfoContainerStyle, 2);
    lv_style_set_pad_row(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_column(&gpsInfoContainerStyle, 0);
    lv_style_set_border_side(&gpsInfoContainerStyle, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&gpsInfoContainerStyle, 1);

    lv_style_init(&gpsInfoTextStyle);
    lv_style_set_text_font(&gpsInfoTextStyle, &martian_mono_16);

    lv_style_init(&lapTimerInfoContainerStyle);
    lv_style_set_radius(&lapTimerInfoContainerStyle, 0);
    lv_style_set_pad_all(&lapTimerInfoContainerStyle, 0);
    lv_style_set_pad_top(&lapTimerInfoContainerStyle, 2);
    lv_style_set_pad_row(&lapTimerInfoContainerStyle, 0);
    lv_style_set_pad_column(&lapTimerInfoContainerStyle, 0);
    lv_style_set_border_side(&lapTimerInfoContainerStyle, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_width(&lapTimerInfoContainerStyle, 1);

    lv_style_init(&lapTimerInfoTextStyle);
    lv_style_set_text_font(&lapTimerInfoTextStyle, &martian_mono_16);
}

void SectorTimesLayout::updateSectorTimes(void *param) {
    auto *data = static_cast<SectorTimeLayoutUpdateInternalData *>(param);
    if (data) {
        data->sector1.widget->update(data->sector1.data);
        data->sector2.widget->update(data->sector2.data);
        data->sector3.widget->update(data->sector3.data);

        updateGpsInfoWidget(data->gpsInfo);

        updateLapTimerInfoWidget(data->lapTimerInfo);
    }

    delete data;
}

void SectorTimesLayout::update(const SectorTimeLayoutUpdateData &updateData) const {
    spdlog::trace("Display.updateSectorTimes -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new SectorTimeLayoutUpdateInternalData{
        {sectorWidget1, updateData.sector1},
        {sectorWidget2, updateData.sector2},
        {sectorWidget3, updateData.sector3},

        {gpsInfoWidget, updateData.gpsInfo},
        {lapTimerInfoWidget, updateData.lapTimerInfo}
    };

    lv_async_call(updateSectorTimes, data);

    spdlog::trace("Display.updateSectorTimes -- Update scheduled");
}

GpsInfoWidget SectorTimesLayout::createGpsInfoWidget(lv_obj_t *parent) const {
    lv_obj_t *flexContainer = lv_obj_create(parent);
    lv_obj_add_style(flexContainer, &gpsInfoContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(flexContainer, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(flexContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(flexContainer, colorSlowerThanPrevious, LV_PART_MAIN);

    lv_obj_t *satStatsLabel = lv_label_create(flexContainer);
    lv_obj_set_size(satStatsLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(satStatsLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(satStatsLabel, "GP: ");

    lv_obj_t *fixInfoLabel = lv_label_create(flexContainer);
    lv_obj_set_size(fixInfoLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(fixInfoLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(fixInfoLabel, "Fix: ");

    lv_obj_t *timeLabel = lv_label_create(flexContainer);
    lv_obj_set_size(timeLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(timeLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(timeLabel, "Time: ");

    lv_obj_t *latLabel = lv_label_create(flexContainer);
    lv_obj_set_size(latLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(latLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(latLabel, "Lat: ");

    lv_obj_t *lonLabel = lv_label_create(flexContainer);
    lv_obj_set_size(lonLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(lonLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(lonLabel, "Lon: ");

    lv_obj_t *speedLabel = lv_label_create(flexContainer);
    lv_obj_set_size(speedLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(speedLabel, &gpsInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(speedLabel, "Speed: ");

    return GpsInfoWidget{flexContainer, latLabel, lonLabel, speedLabel, fixInfoLabel, timeLabel, satStatsLabel};
}

LapTimerInfoWidget SectorTimesLayout::createLapTimerInfoWidget(lv_obj_t *parent) const {
    lv_obj_t *flexContainer = lv_obj_create(parent);
    lv_obj_add_style(flexContainer, &lapTimerInfoContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(flexContainer, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(flexContainer, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_color(flexContainer, colorFasterThanPrevious, LV_PART_MAIN);

    lv_obj_t *lapCounterLabel = lv_label_create(flexContainer);
    lv_obj_set_size(lapCounterLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(lapCounterLabel, &lapTimerInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(lapCounterLabel, "Lap: ");

    lv_obj_t *distanceLabel = lv_label_create(flexContainer);
    lv_obj_set_size(distanceLabel, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_add_style(distanceLabel, &lapTimerInfoTextStyle, LV_PART_MAIN);
    lv_label_set_text(distanceLabel, "Distance: ");

    return LapTimerInfoWidget{flexContainer, distanceLabel, lapCounterLabel};
}

void SectorTimesLayout::updateGpsInfoWidget(GpsInfoWidgetData &gpsInfoWidgetData) {
    spdlog::trace("Display.updateGpsInfoWidget -- Started");

    std::ostringstream oss;

    oss << "Fix: " << (gpsInfoWidgetData.data.hasFix ? 1 : 0) << "  "
            << "Sat: " << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.numberOfSatellites << "  "
            << "Hdop: " << std::fixed << std::setprecision(2) << gpsInfoWidgetData.data.hdop;
    lv_label_set_text(gpsInfoWidgetData.widget.fixInfoLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "P:" << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.gpsSats << " "
            << "L:" << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.glonassSats << " "
            << "A:" << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.galileoSats << " "
            << "B:" << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.beidouSats << " "
            << "Q:" << std::setw(2) << std::setfill(' ') << gpsInfoWidgetData.data.qzssSats;
    lv_label_set_text(gpsInfoWidgetData.widget.satStatsLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Time:"
            // << " " << std::setw(2) << std::setfill('0') << gpsInfoWidgetData.data.day
            // << "." << std::setw(2) << std::setfill('0') << gpsInfoWidgetData.data.month
            // << "." << std::setw(4) << std::setfill('0') << gpsInfoWidgetData.data.year
            << " " << std::setw(2) << std::setfill('0') << gpsInfoWidgetData.data.hours
            << ":" << std::setw(2) << std::setfill('0') << gpsInfoWidgetData.data.minutes
            << ":" << std::setw(2) << std::setfill('0') << gpsInfoWidgetData.data.seconds
            << "." << std::setw(3) << std::setfill('0') << gpsInfoWidgetData.data.microseconds;
    lv_label_set_text(gpsInfoWidgetData.widget.timeLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Lat: " << std::fixed << std::setprecision(6) << gpsInfoWidgetData.data.latitude;
    lv_label_set_text(gpsInfoWidgetData.widget.latLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Lon: " << std::fixed << std::setprecision(6) << gpsInfoWidgetData.data.longitude;
    lv_label_set_text(gpsInfoWidgetData.widget.lonLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Speed: " << std::fixed << std::setprecision(2) << gpsInfoWidgetData.data.speed;
    lv_label_set_text(gpsInfoWidgetData.widget.speedLabel, oss.str().c_str());

    spdlog::trace("Display.updateGpsInfoWidget -- Finished");
}

void SectorTimesLayout::updateLapTimerInfoWidget(LapTimerInfoWidgetData &lapTimerInfoWidgetData) {
    spdlog::trace("Display.updateLapTimerInfoWidget -- Started");

    std::ostringstream oss;

    oss << "Lap: " << std::setw(2) << std::setfill(' ') << lapTimerInfoWidgetData.data.lapCounter;
    lv_label_set_text(lapTimerInfoWidgetData.widget.lapCounterLabel, oss.str().c_str());

    oss.str("");
    oss.clear();
    oss << "Distance: " << std::fixed << std::setprecision(6) << lapTimerInfoWidgetData.data.distance;
    lv_label_set_text(lapTimerInfoWidgetData.widget.distanceLabel, oss.str().c_str());

    spdlog::trace("Display.updateLapTimerInfoWidget -- Finished");
}
