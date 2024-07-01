#ifndef RACE_COMPUTER_GPSMODELS_H
#define RACE_COMPUTER_GPSMODELS_H

#include <string>
#include <variant>
#include <vector>

struct GpsUpdateBase {
    std::string nmeaSentence;
};

struct PositionUpdate : GpsUpdateBase {
    bool hasFix{};
    float latitude{};
    float longitude{};
};

struct TimeUpdate : GpsUpdateBase {
    int hour{};
    int minute{};
    int second{};
};

struct SpeedUpdate : GpsUpdateBase {
    float speed{};
};

struct SatellitesUpdate : GpsUpdateBase {
    int numberOfSatellites{};
    float hdop{};
};

struct GpsState {
    bool hasFix{};
    int fixQuality{-1};
    int numberOfSatellites{-1};
    float hdop{-1};

    int gpsSats{-1};
    int glonassSats{-1};
    int galileoSats{-1};
    int beidouSats{-1};
    int qzssSats{-1};

    int day{-1};
    int month{-1};
    int year{-1};

    int hours{-1};
    int minutes{-1};
    int seconds{-1};
    int microseconds{-1};

    float latitude{-1};
    float longitude{-1};

    float speed{-1};
};

using GpsUpdate = std::variant<PositionUpdate, TimeUpdate, SpeedUpdate, SatellitesUpdate>;
using GpsUpdateList = std::vector<GpsUpdate>;

#endif //RACE_COMPUTER_GPSMODELS_H
