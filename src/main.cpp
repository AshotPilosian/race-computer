#include <iomanip>
#include <iostream>
#include <sstream>
#include <wiringx.h>
#include <unistd.h>
#include <variant>

#include "spdlog/spdlog.h"
#include "gps/GPS.h"
#include "display/Display.h"
#include "display/layout/SectorTimesLayout.h"

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

GPS gps;
Display display;
SectorTimesLayout sectorTimesLayout(&display);

int displayUpdateIntervalMs = 250;
long long displayUpdatedAtMs = 0;

void setup() {
    spdlog::info("Setup started");

    display.setup();
    sectorTimesLayout.setup();
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
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, PositionUpdate>) {
            spdlog::info("Position Update. NMEA: {}; Lat: {}; Lon: {}", arg.nmeaSentence, arg.latitude, arg.longitude);
        } else if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, SpeedUpdate>) {
            spdlog::info("Speed Update. Speed: {}", arg.speed);
        }
    }, update);
}

long long getCurrentTimeMs() {
    // Get the current time point from the system clock
    const auto now = std::chrono::system_clock::now();

    // Convert the time point to a duration since epoch
    const auto duration = duration_cast<std::chrono::milliseconds>(now.time_since_epoch());

    // Get the number of milliseconds
    return duration.count();
}

void updateDisplayIfNeeded() {
    if (long long currentTimeMs = getCurrentTimeMs();
        currentTimeMs - displayUpdatedAtMs > displayUpdateIntervalMs) {
        spdlog::debug("Updating info on display");

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << "+" << (rand() % 50) / 100.0;

        const SectorTimeLayoutUpdateData updateData = {
            SectorTimeUpdateData{FASTER_THEN_PREVIOUS_LAP, "-0.40"},
            SectorTimeUpdateData{SLOWER_THEN_PREVIOUS_LAP, oss.str()},
            SectorTimeUpdateData{BEST, "-0.07"}
        };
        sectorTimesLayout.update(updateData);

        displayUpdatedAtMs = currentTimeMs;
    }
}

int main() {
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S.%e] [%^%L%$] %v");

    setup();

    spdlog::info("Application started");

    int updatedDataCounter = 0;
    while (updatedDataCounter < 100) {
        spdlog::trace("Main loop iteration started");

        gps.readAvailable();
        if (auto unprocessedUpdates = gps.getUnprocessedUpdates()) {
            for (const auto &update: unprocessedUpdates.value()) {
                handleUpdate(update);

                updatedDataCounter++;
            }
        }

        updateDisplayIfNeeded();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    spdlog::info("Application finished");

    shutdown();

    return 0;
}
