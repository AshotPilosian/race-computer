#ifndef RACE_COMPUTER_DISPLAY_H
#define RACE_COMPUTER_DISPLAY_H

#include <atomic>
#include <thread>

#include "DisplayModels.h"
#include "lvgl.h"

class Display {
    lv_style_t sectorTimeTextStyle;
    lv_style_t sectorTimeContainerStyle;
    lv_style_t mainContainerStyle;

    lv_obj_t *mainContainer;
    SectorTimeWidget sectorWidget1;
    SectorTimeWidget sectorWidget2;
    SectorTimeWidget sectorWidget3;

    std::thread lvglThread;
    std::atomic<bool> running;

    void lvglMainLoop();

    void initLvgl();

    void initStyles();

    SectorTimeWidget createSectorTimeWidget(lv_obj_t *parent, int32_t width, int32_t offsetX) const;

    static void updateSectorTimes(void *param);

    static void resetScreenColor(void *arg);

    static void updateSectorTimeWidget(SectorTimeWidgetData &sectorTimeWidgetData);

public:
    Display();

    void setup();

    void update() const;

    void shutdown();
};


#endif //RACE_COMPUTER_DISPLAY_H
