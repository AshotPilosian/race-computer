#ifndef COLOREDSECTORTIMESPANEL_H
#define COLOREDSECTORTIMESPANEL_H

#include <lvgl.h>

#include "../../widgets/sector/SectorTimeColoredWidget.h"

struct SectorTimesUpdateData {
    SectorTimeUpdateData sector1;
    SectorTimeUpdateData sector2;
    SectorTimeUpdateData sector3;
};

struct ColoredSectorTimesPanelInternal {
    lv_obj_t *container;
    SectorTimeColoredWidget *sector1;
    SectorTimeColoredWidget *sector2;
    SectorTimeColoredWidget *sector3;
};

class ColoredSectorTimesPanel {
    lv_obj_t *parent;
    int32_t height;

    lv_style_t sectorTimesContainerStyle;
    ColoredSectorTimesPanelInternal widget{};

    void initStyles();

    void init();

public:
    ColoredSectorTimesPanel(lv_obj_t *parent, int32_t height);
    ~ColoredSectorTimesPanel();

    void update(const SectorTimesUpdateData &data) const;
};

#endif //COLOREDSECTORTIMESPANEL_H
