#ifndef RACE_COMPUTER_GPS_H
#define RACE_COMPUTER_GPS_H

#include <optional>
#include <cstdio>
#include <cstdint>


class GPS {
private:
    int uartFd;

    uint8_t calculateChecksum(const char *sentence);

    std::optional <uint8_t> extractChecksum(const char *sentence);

    bool validateChecksum(const char *sentence);

public:
    GPS();

    void setup();

    void start();

    void shutdown();
};


#endif //RACE_COMPUTER_GPS_H
