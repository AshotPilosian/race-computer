#ifndef RACE_COMPUTER_GPS_H
#define RACE_COMPUTER_GPS_H

#include <optional>
#include <cstdint>
#include <span>
#include "GpsModels.h"

class GPS {
    int uartFd;
    char nmeaBuffer[1000];
    int nmeaBufferCurrentIdx;
    bool nmeaBufferStartValid;

    std::optional <GpsUpdateList> unprocessedUpdates;

    static uint8_t calculateChecksum(const char *sentence);

    static std::optional <uint8_t> extractChecksum(const char *sentence);

    static bool validateChecksum(const char *sentence);

    static int openUartToGpsModule(unsigned int baudRate);

    void writeCommandToModule(std::span<const unsigned char> commandBytes) const;

public:
    GPS();

    GpsState currentState;

    void setup();

    void readAvailable();

    std::optional <GpsUpdateList> getUnprocessedUpdates();

    void shutdown() const;
};


#endif //RACE_COMPUTER_GPS_H
