#ifndef SECTORTIMESLAYOUT_H
#define SECTORTIMESLAYOUT_H

#include <lvgl.h>

#include "../Display.h"
#include "LayoutModels.h"
#include "panels/gps/GpsPositionExtendedInfoPanel.h"
#include "panels/laptimer/SimpleLapTimerInfoPanel.h"
#include "panels/sector/ColoredSectorTimesPanel.h"

struct ColoredSectorTimesPanelUpdateData {
    ColoredSectorTimesPanel *widget{};
    SectorTimesUpdateData data;
};

struct GpsPositionExtendedInfoPanelUpdateData {
    GpsPositionExtendedInfoPanel *widget{};
    GpsState data;
};

struct SimpleLapTimerInfoPanelUpdateData {
    SimpleLapTimerInfoPanel *widget{};
    LapTimerUpdateData data;
};

struct SectorTimeLayoutUpdateInternalData {
    ColoredSectorTimesPanelUpdateData sectors{};
    GpsPositionExtendedInfoPanelUpdateData gpsInfo{};
    SimpleLapTimerInfoPanelUpdateData lapTimerInfo{};
};

class SectorTimesLayout {
    Display *display;

    lv_obj_t *mainContainer;

    ColoredSectorTimesPanel *sectorsPanel;
    GpsPositionExtendedInfoPanel *gpsInfoPanel;
    SimpleLapTimerInfoPanel *lapTimerPanel;

    lv_style_t mainContainerStyle;

public:
    explicit SectorTimesLayout(Display *_display);

    ~SectorTimesLayout();

    void setup();

    void initStyles();

    void update(const SectorTimeLayoutUpdateData &updateData) const;

    static void updateSectorTimes(void *param);
};


#endif //SECTORTIMESLAYOUT_H
