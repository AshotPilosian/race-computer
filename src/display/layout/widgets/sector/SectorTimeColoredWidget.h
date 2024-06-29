#ifndef SECTORTIMECOLOREDWIDGET_H
#define SECTORTIMECOLOREDWIDGET_H

#include <lvgl.h>
#include <string>

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

struct SectorTimeColoredWidgetInternal {
    lv_obj_t *container;
    lv_obj_t *label;
};

class SectorTimeColoredWidget {
    lv_style_t sectorTimeTextStyle{};
    lv_style_t sectorTimeContainerStyle{};
    SectorTimeColoredWidgetInternal widget{};

    void initStyles();

    void initWidget(lv_obj_t *parent, int32_t width);

    static lv_color_t toColor(SectorState sectorState);

public:
    SectorTimeColoredWidget(lv_obj_t *parent, int32_t width);

    void update(const SectorTimeUpdateData &sectorTimeUpdateData) const;
};

#endif //SECTORTIMECOLOREDWIDGET_H
