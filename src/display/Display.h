#ifndef RACE_COMPUTER_DISPLAY_H
#define RACE_COMPUTER_DISPLAY_H

#include <atomic>
#include <thread>
#include <mutex>

class Display {
    std::thread lvglThread;
    std::atomic<bool> running;

    void lvglMainLoop();

    void initLvgl();

    static void resetScreenColor(void *arg);

public:
    std::mutex lvgl_mutex;

    Display();

    bool initialized;

    void setup();

    void shutdown();
};


#endif //RACE_COMPUTER_DISPLAY_H
