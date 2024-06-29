#ifndef SECTORTIMESLAYOUT_H
#define SECTORTIMESLAYOUT_H

#include <lvgl.h>

#include "../Display.h"
#include "LayoutModels.h"

struct SectorTimeColoredWidgetUpdateData {
    SectorTimeColoredWidget *widget;
    SectorTimeUpdateData data;
};

struct SectorTimeLayoutUpdateInternalData {
    SectorTimeColoredWidgetUpdateData sector1{};
    SectorTimeColoredWidgetUpdateData sector2{};
    SectorTimeColoredWidgetUpdateData sector3{};

    GpsInfoWidgetData gpsInfo;
    LapTimerInfoWidgetData lapTimerInfo{};
};

class SectorTimesLayout {
    Display *display;

    lv_obj_t *mainContainer;

    SectorTimeColoredWidget *sectorWidget1;
    SectorTimeColoredWidget *sectorWidget2;
    SectorTimeColoredWidget *sectorWidget3;

    GpsInfoWidget gpsInfoWidget;

    LapTimerInfoWidget lapTimerInfoWidget;

    lv_style_t mainContainerStyle;
    lv_style_t gpsInfoContainerStyle;
    lv_style_t gpsInfoTextStyle;
    lv_style_t lapTimerInfoContainerStyle;
    lv_style_t lapTimerInfoTextStyle;
    lv_style_t sectorTimesContainerStyle;

public:
    explicit SectorTimesLayout(Display *_display);
    ~SectorTimesLayout();

    void setup();

    void initStyles();

    void update(const SectorTimeLayoutUpdateData &updateData) const;

    GpsInfoWidget createGpsInfoWidget(lv_obj_t *parent) const;

    LapTimerInfoWidget createLapTimerInfoWidget(lv_obj_t *parent) const;

    static void updateSectorTimes(void *param);

    static void updateGpsInfoWidget(GpsInfoWidgetData &gpsInfoWidgetData);

    static void updateLapTimerInfoWidget(LapTimerInfoWidgetData &lapTimerInfoWidgetData);
};


#endif //SECTORTIMESLAYOUT_H
