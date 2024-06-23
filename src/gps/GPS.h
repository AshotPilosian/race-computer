#ifndef RACE_COMPUTER_GPS_H
#define RACE_COMPUTER_GPS_H

#include <optional>
#include <cstdint>
#include <span>
#include "GpsModels.h"

class GPS {
private:
    int uartFd;
    char nmeaBuffer[100];
    int nmeaBufferCurrentIdx;
    bool nmeaBufferStartValid;

    std::optional <GpsUpdateList> unprocessedUpdates;

    uint8_t calculateChecksum(const char *sentence);

    std::optional <uint8_t> extractChecksum(const char *sentence);

    bool validateChecksum(const char *sentence);

    int openUartToGpsModule(unsigned int baudRate);

    void writeCommandToModule(std::span<const unsigned char> commandBytes) const;

public:
    GPS();

    GpsState currentState;

    void setup();

    void readAvailable();

    std::optional <GpsUpdateList> getUnprocessedUpdates();

    void shutdown();
};


#endif //RACE_COMPUTER_GPS_H
