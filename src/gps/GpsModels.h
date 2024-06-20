#ifndef RACE_COMPUTER_GPSMODELS_H
#define RACE_COMPUTER_GPSMODELS_H

struct GpsUpdateBase {
    std::string nmeaSentence;
};

struct PositionUpdate : GpsUpdateBase {
    float latitude;
    float longitude;
};

struct TimeUpdate : GpsUpdateBase {
    int hour;
    int minute;
    int second;
};

struct SpeedUpdate : GpsUpdateBase {
    float speed;
};

struct SatellitesUpdate : GpsUpdateBase {
    int numberOfSatellites;
    float hdop;
};

using GpsUpdate = std::variant<PositionUpdate, TimeUpdate, SpeedUpdate, SatellitesUpdate>;
using GpsUpdateList = std::vector<GpsUpdate>;

#endif //RACE_COMPUTER_GPSMODELS_H
