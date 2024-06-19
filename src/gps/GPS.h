#ifndef RACE_COMPUTER_GPS_H
#define RACE_COMPUTER_GPS_H

#include <optional>
#include <cstdio>
#include <cstdint>
#include <string>
#include <variant>

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


class GPS {
private:
    int uartFd;
    char nmeaBuffer[100];
    int nmeaBufferCurrentIdx;
    bool nmeaBufferStartValid;
    std::optional <GpsUpdate> unprocessedUpdate;

    uint8_t calculateChecksum(const char *sentence);

    std::optional <uint8_t> extractChecksum(const char *sentence);

    bool validateChecksum(const char *sentence);

public:
    GPS();

    void setup();

    void readAvailable();

    std::optional <GpsUpdate> getUnprocessedUpdate();

    void shutdown();
};


#endif //RACE_COMPUTER_GPS_H
