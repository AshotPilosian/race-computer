#include <iostream>
#include <wiringx.h>
#include <unistd.h>
#include <string.h>
#include <variant>

#include "spdlog/spdlog.h"
#include "gps/GPS.h"
#include "display/Display.h"

int init_wiringX() {
    spdlog::info("Setting up wiringX");

    system("duo-pinmux -w GP2/UART1_TX && duo-pinmux -w GP3/UART1_RX");

    if (wiringXSetup("milkv_duo256m", NULL) == -1) {
        wiringXGC();
        return 1;
    }
    sleep(1);
    spdlog::info("wiringX setup completed");

    return 0;
}

Display display;
GPS gps;

void setup() {
    spdlog::info("Setup started");

    display.setup();
    init_wiringX();
    gps.setup();

    spdlog::info("Setup finished");
}

void shutdown() {
    spdlog::info("Shutdown started");

    display.shutdown();
    gps.shutdown();

    spdlog::info("Shutdown finished");
}

void handleUpdate(const GpsUpdate &update) {
    std::visit([](const auto &arg) {
        if constexpr(std::is_same_v < std::decay_t < decltype(arg) > , PositionUpdate > )
        {
            spdlog::info("Position Update. Lat: {}; :Lon: {}; NMEA: {}", arg.nmeaSentence, arg.latitude, arg.longitude);
        } else if constexpr(std::is_same_v < std::decay_t < decltype(arg) > , SpeedUpdate > )
        {
            spdlog::info("Speed Update. Speed: {}", arg.speed);
        }
    }, update);
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%L%$] %v");

    setup();

    spdlog::info("Application started");
    int updatedDataCounter = 0;
    while (updatedDataCounter < 10) {
        spdlog::trace("Main loop iteration started");

        gps.readAvailable();
        auto unprocessedUpdates = gps.getUnprocessedUpdates();
        if (unprocessedUpdates) {
            for (const auto &update: unprocessedUpdates.value()) {
                handleUpdate(update);

                updatedDataCounter++;
            }
        }

        usleep(1000);
    }
    spdlog::info("Application finished");

    shutdown();

    return 0;
}