#ifndef DISPLAYMODELS_H
#define DISPLAYMODELS_H

#include "lvgl.h"

const lv_color_t colorFastestOverall = lv_color_hex(0xff76ff);
const lv_color_t colorFasterThanPrevious = lv_color_hex(0x00ff00);
const lv_color_t colorSlowerThanPrevious = lv_color_hex(0xffff00);
const lv_color_t colorGrey = lv_color_hex(0xeeeeee);

struct SectorTimeWidget {
    lv_obj_t *container;
    lv_obj_t *label;
};

struct SectorTimeWidgetData {
    SectorTimeWidget widget;
    lv_color_t color;
    std::string text;
};

struct LayoutUpdateData {
    SectorTimeWidgetData sector1;
    SectorTimeWidgetData sector2;
    SectorTimeWidgetData sector3;
    SectorTimeWidgetData sector4;
};

#endif //DISPLAYMODELS_H
