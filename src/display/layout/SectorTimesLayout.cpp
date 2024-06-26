#include "SectorTimesLayout.h"

#include <iomanip>
#include <sstream>

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

    gpsInfoWidget = createGpsInfoWidget(mainContainer, lv_pct(20));

    std::this_thread::sleep_for(std::chrono::seconds(1));

    spdlog::info("SectorTimesLayout setup completed");
}

void SectorTimesLayout::initStyles() {
    LV_FONT_DECLARE(martian_mono_16);
    LV_FONT_DECLARE(martian_mono_20);

    lv_style_init(&mainContainerStyle);
    lv_style_set_border_width(&mainContainerStyle, 0);
    lv_style_set_radius(&mainContainerStyle, 0);
    lv_style_set_pad_all(&mainContainerStyle, 0);

    lv_style_init(&sectorTimeTextStyle);
    lv_style_set_text_font(&sectorTimeTextStyle, &martian_mono_20);

    lv_style_init(&sectorTimeContainerStyle);
    lv_style_set_radius(&sectorTimeContainerStyle, 0);
    lv_style_set_pad_all(&sectorTimeContainerStyle, 0);
    lv_style_set_border_side(&sectorTimeContainerStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT);
    lv_style_set_border_width(&sectorTimeContainerStyle, 1);

    lv_style_init(&gpsInfoContainerStyle);
    lv_style_set_radius(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_all(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_top(&gpsInfoContainerStyle, 3);
    lv_style_set_pad_row(&gpsInfoContainerStyle, 0);
    lv_style_set_pad_column(&gpsInfoContainerStyle, 0);
    lv_style_set_border_side(&gpsInfoContainerStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_LEFT);
    lv_style_set_border_width(&gpsInfoContainerStyle, 1);

    lv_style_init(&gpsInfoTextStyle);
    lv_style_set_text_font(&gpsInfoTextStyle, &martian_mono_16);
}

void SectorTimesLayout::updateSectorTimes(void *param) {
    auto *data = static_cast<SectorTimeLayoutUpdateInternalData *>(param);
    if (data) {
        updateSectorTimeWidget(data->sector1);
        updateSectorTimeWidget(data->sector2);
        updateSectorTimeWidget(data->sector3);

        updateGpsInfoWidget(data->gpsInfo);
    }

    delete data;
}

void SectorTimesLayout::update(const SectorTimeLayoutUpdateData &updateData) const {
    spdlog::trace("Display.updateSectorTimes -- Schedule update");

    // Allocate the data on the heap to ensure it remains valid for async call
    auto *data = new SectorTimeLayoutUpdateInternalData{
        {sectorWidget1, toColor(updateData.sector1.state), updateData.sector1.time},
        {sectorWidget2, toColor(updateData.sector2.state), updateData.sector2.time},
        {sectorWidget3, toColor(updateData.sector3.state), updateData.sector3.time},
        {gpsInfoWidget, updateData.gpsInfo}
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

GpsInfoWidget SectorTimesLayout::createGpsInfoWidget(lv_obj_t *parent, const int32_t offsetY) const {
    lv_obj_t *flexContainer = lv_obj_create(lv_scr_act());
    lv_obj_add_style(flexContainer, &gpsInfoContainerStyle, LV_PART_MAIN);
    lv_obj_set_size(flexContainer, lv_pct(100), lv_pct(50));
    lv_obj_set_pos(flexContainer, 0, offsetY);
    lv_obj_set_flex_flow(flexContainer, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(flexContainer, LV_SCROLLBAR_MODE_OFF);

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

void SectorTimesLayout::updateSectorTimeWidget(SectorTimeWidgetData &sectorTimeWidgetData) {
    spdlog::trace("Display.updateSectorTimeWidget -- Started");

    lv_obj_set_style_bg_color(sectorTimeWidgetData.widget.container, sectorTimeWidgetData.color, LV_PART_MAIN);
    spdlog::trace("Display.updateSectorTimeWidget -- Set color");

    lv_label_set_text(sectorTimeWidgetData.widget.label, sectorTimeWidgetData.text.data());
    spdlog::trace("Display.updateSectorTimeWidget -- Set text: {}", sectorTimeWidgetData.text);

    spdlog::trace("Display.updateSectorTimeWidget -- Finished");
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
