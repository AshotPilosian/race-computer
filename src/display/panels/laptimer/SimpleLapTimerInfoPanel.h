#ifndef SIMPLELAPTIMERINFOPANEL_H
#define SIMPLELAPTIMERINFOPANEL_H

#include <lvgl.h>

struct LapTimerUpdateData {
    int lapCounter;
    double distance;
};

struct SimpleLapTimerInfoPanelInternal {
    lv_obj_t *container;
    lv_obj_t *distanceLabel;
    lv_obj_t *lapCounterLabel;
};

class SimpleLapTimerInfoPanel {
    lv_obj_t *parent;

    lv_style_t containerStyle{};
    lv_style_t textStyle{};
    SimpleLapTimerInfoPanelInternal widget{};

    void initStyles();

    void init();

public:
    explicit SimpleLapTimerInfoPanel(lv_obj_t *parent);

    void update(const LapTimerUpdateData *data) const;
};



#endif //SIMPLELAPTIMERINFOPANEL_H
