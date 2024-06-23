#ifndef LAYOUTMODELS_H
#define LAYOUTMODELS_H

#include <string>

#include "lvgl.h"

const lv_color_t colorFastestOverall = lv_color_hex(0xff76ff);
const lv_color_t colorFasterThanPrevious = lv_color_hex(0x00ff00);
const lv_color_t colorSlowerThanPrevious = lv_color_hex(0xffff00);
const lv_color_t colorGrey = lv_color_hex(0xeeeeee);
const lv_color_t colorLightGrey = lv_color_hex(0xf5f5f5);

struct SectorTimeWidget {
    lv_obj_t *container;
    lv_obj_t *label;
};

struct SectorTimeWidgetData {
    SectorTimeWidget widget;
    lv_color_t color;
    std::string text;
};

struct SectorTimeLayoutUpdateInternalData {
    SectorTimeWidgetData sector1;
    SectorTimeWidgetData sector2;
    SectorTimeWidgetData sector3;
};

enum SectorState {
    NOT_STARTED,
    IN_PROGRESS,
    SLOWER_THEN_PREVIOUS_LAP,
    FASTER_THEN_PREVIOUS_LAP,
    BEST
};

struct SectorTimeUpdateData {
    SectorState state;
    std::string time;
};

struct SectorTimeLayoutUpdateData {
    SectorTimeUpdateData sector1;
    SectorTimeUpdateData sector2;
    SectorTimeUpdateData sector3;
};

#endif //LAYOUTMODELS_H
