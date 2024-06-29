#ifndef LAYOUTMODELS_H
#define LAYOUTMODELS_H

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

struct SectorTimeLayoutUpdateData {
    SectorTimeUpdateData sector1;
    SectorTimeUpdateData sector2;
    SectorTimeUpdateData sector3;

    GpsState gpsInfo;

    LapTimerInfoUpdateData lapTimerInfo{};
};

#endif //LAYOUTMODELS_H
