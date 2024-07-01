#ifndef LAPTIMER_H
#define LAPTIMER_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class LapTimer {
    double startLat1, startLon1;
    double startLat2, startLon2;
    int lapCounter;
    double lastLat;
    double lastLon;
    bool firstPosition;
    double latestDistance;

    bool crossedStartFinishLine(double lat1, double lon1,
                                double lat2, double lon2) const;

    static double haversineDistance(double lat1, double lon1,
                                    double lat2, double lon2);

    static double orientation(double lat1, double lon1,
                              double lat2, double lon2,
                              double lat3, double lon3);

public:
    LapTimer(const double startLat1, const double startLon1,
             const double startLat2, const double startLon2): startLat1(startLat1),
                                                              startLon1(startLon1),
                                                              startLat2(startLat2),
                                                              startLon2(startLon2),
                                                              lapCounter(-1),
                                                              lastLat(0.0),
                                                              lastLon(0.0),
                                                              firstPosition(true),
                                                              latestDistance(-1.0) {
    }

    void addPosition(double latitude, double longitude);

    int getNumberOfLaps() const;

    double getLatestDistance() const;
};


#endif //LAPTIMER_H
