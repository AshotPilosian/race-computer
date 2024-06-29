#ifndef GPSPOSITIONEXTENDEDINFOWIDGET_H
#define GPSPOSITIONEXTENDEDINFOWIDGET_H

#include <lvgl.h>

#include "../gps/GpsModels.h"

struct GpsPositionExtendedInfoPanelInternal {
    lv_obj_t *container;
    lv_obj_t *latLabel;
    lv_obj_t *lonLabel;
    lv_obj_t *speedLabel;
    lv_obj_t *fixInfoLabel;
    lv_obj_t *timeLabel;
    lv_obj_t *satStatsLabel;
};

class GpsPositionExtendedInfoPanel {
    lv_obj_t *parent;

    lv_style_t containerStyle{};
    lv_style_t textStyle{};
    GpsPositionExtendedInfoPanelInternal widget{};

    void initStyles();

    void init();

public:
    explicit GpsPositionExtendedInfoPanel(lv_obj_t *parent);

    void update(const GpsState *data) const;
};


#endif //GPSPOSITIONEXTENDEDINFOWIDGET_H
