#ifndef LAYOUTMODELS_H
#define LAYOUTMODELS_H

#include "../gps/GpsModels.h"
#include "panels/laptimer/SimpleLapTimerInfoPanel.h"
#include "widgets/sector/SectorTimeColoredWidget.h"

struct SectorTimeLayoutUpdateData {
    SectorTimeUpdateData sector1;
    SectorTimeUpdateData sector2;
    SectorTimeUpdateData sector3;

    GpsState gpsInfo;

    LapTimerUpdateData lapTimerInfo{};
};

#endif //LAYOUTMODELS_H
