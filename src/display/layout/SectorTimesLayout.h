#ifndef SECTORTIMESLAYOUT_H
#define SECTORTIMESLAYOUT_H

#include <lvgl.h>

#include "../Display.h"
#include "../panels/gps/GpsPositionExtendedInfoPanel.h"
#include "../panels/sector/ColoredSectorTimesPanel.h"
#include "../panels/laptimer/SimpleLapTimerInfoPanel.h"
#include "LayoutModels.h"

struct ColoredSectorTimesPanelUpdateData {
    const ColoredSectorTimesPanel *widget{};
    const SectorTimesUpdateData *data{};
};

struct GpsPositionExtendedInfoPanelUpdateData {
    const GpsPositionExtendedInfoPanel *widget{};
    const GpsState *data{};
};

struct SimpleLapTimerInfoPanelUpdateData {
    const SimpleLapTimerInfoPanel *widget{};
    const LapTimerUpdateData *data{};
};

struct SectorTimeLayoutUpdateInternalData {
    const ColoredSectorTimesPanelUpdateData *sectors{};
    const GpsPositionExtendedInfoPanelUpdateData *gpsInfo{};
    const SimpleLapTimerInfoPanelUpdateData *lapTimerInfo{};
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

    void update(const SectorTimeLayoutUpdateData *updateData) const;
};


#endif //SECTORTIMESLAYOUT_H
