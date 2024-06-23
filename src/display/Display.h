#ifndef RACE_COMPUTER_DISPLAY_H
#define RACE_COMPUTER_DISPLAY_H

#include <atomic>
#include <thread>

class Display {
    std::thread lvglThread;
    std::atomic<bool> running;

    void lvglMainLoop() const;

    void initLvgl();

    static void resetScreenColor(void *arg);

public:
    Display();

    bool initialized;

    void setup();

    void shutdown();
};


#endif //RACE_COMPUTER_DISPLAY_H
