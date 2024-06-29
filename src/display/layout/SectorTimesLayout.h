#ifndef SECTORTIMESLAYOUT_H
#define SECTORTIMESLAYOUT_H

#include <lvgl.h>

#include "../Display.h"
#include "../panels/gps/GpsPositionExtendedInfoPanel.h"
#include "../panels/sector/ColoredSectorTimesPanel.h"
#include "../panels/laptimer/SimpleLapTimerInfoPanel.h"
#include "LayoutModels.h"

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

    void initStyles();

    void init();

    static void doUpdate(void *param);

public:
    explicit SectorTimesLayout(Display *display);

    ~SectorTimesLayout();

    void setup();

    void update(const SectorTimeLayoutUpdateData &updateData) const;
};


#endif //SECTORTIMESLAYOUT_H
