#ifndef LAYOUTMODELS_H
#define LAYOUTMODELS_H

#include <string>

#include "lvgl.h"
#include "../gps/GpsModels.h"
#include "widgets/sector/SectorTimeColoredWidget.h"

struct LapTimerInfoUpdateData {
    int lapCounter;
    double distance;
};

struct LapTimerInfoWidget {
    lv_obj_t *container;
    lv_obj_t *distanceLabel;
    lv_obj_t *lapCounterLabel;
};

struct LapTimerInfoWidgetData {
    LapTimerInfoWidget widget;
    LapTimerInfoUpdateData data;
};

struct GpsInfoWidget {
    lv_obj_t *container;
    lv_obj_t *latLabel;
    lv_obj_t *lonLabel;
    lv_obj_t *speedLabel;
    lv_obj_t *fixInfoLabel;
    lv_obj_t *timeLabel;
    lv_obj_t *satStatsLabel;
};

struct SectorTimeLayoutUpdateData {
    SectorTimeUpdateData sector1;
    SectorTimeUpdateData sector2;
    SectorTimeUpdateData sector3;

    GpsState gpsInfo;

    LapTimerInfoUpdateData lapTimerInfo{};
};

struct GpsInfoWidgetData {
    GpsInfoWidget widget;
    GpsState data;
};

#endif //LAYOUTMODELS_H
