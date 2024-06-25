#ifndef SECTORTIMESLAYOUT_H
#define SECTORTIMESLAYOUT_H

#include <lvgl.h>

#include "../Display.h"
#include "LayoutModels.h"


class SectorTimesLayout {
    Display *display;

    lv_obj_t *mainContainer;

    SectorTimeWidget sectorWidget1;
    SectorTimeWidget sectorWidget2;
    SectorTimeWidget sectorWidget3;

    GpsInfoWidget gpsInfoWidget;

    lv_style_t sectorTimeTextStyle;
    lv_style_t sectorTimeContainerStyle;
    lv_style_t mainContainerStyle;
    lv_style_t gpsInfoContainerStyle;
    lv_style_t gpsInfoTextStyle;

    static lv_color_t toColor(SectorState sectorState);

public:
    explicit SectorTimesLayout(Display *_display);

    void setup();

    void initStyles();

    void update(const SectorTimeLayoutUpdateData &updateData) const;

    SectorTimeWidget createSectorTimeWidget(lv_obj_t *parent, int32_t width, int32_t offsetX) const;

    GpsInfoWidget createGpsInfoWidget(lv_obj_t *parent, const int32_t offsetY) const;

    static void updateSectorTimes(void *param);

    static void updateSectorTimeWidget(SectorTimeWidgetData &sectorTimeWidgetData);

    static void updateGpsInfoWidget(GpsInfoWidgetData &gpsInfoWidgetData);
};


#endif //SECTORTIMESLAYOUT_H
