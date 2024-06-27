#include <cmath>

#include "LapTimer.h"

#include "spdlog/spdlog.h"

bool LapTimer::crossedStartFinishLine(const double lat1, const double lon1,
                                      const double lat2, const double lon2) const {
    // Calculate orientation for both segments
    const double orientation1 = orientation(startLat1, startLon1, startLat2, startLon2, lat1, lon1);
    const double orientation2 = orientation(startLat1, startLon1, startLat2, startLon2, lat2, lon2);

    // Check if there is a sign change indicating a crossing
    return orientation1 * orientation2 < 0;
}

double LapTimer::haversineDistance(double lat1, const double lon1,
                                   double lat2, const double lon2) {
    constexpr double R = 6371000; // Radius of the Earth in meters
    constexpr double toRadians = M_PI / 180.0;

    const double dLat = (lat2 - lat1) * toRadians;
    const double dLon = (lon2 - lon1) * toRadians;

    lat1 *= toRadians;
    lat2 *= toRadians;

    const double a = std::sin(dLat / 2) * std::sin(dLat / 2) +
                     std::cos(lat1) * std::cos(lat2) * std::sin(dLon / 2) * std::sin(dLon / 2);
    const double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return R * c;
}

void LapTimer::addPosition(const double latitude, const double longitude) {
    if (firstPosition) {
        lastLat = latitude;
        lastLon = longitude;
        firstPosition = false;
    } else {
        if (crossedStartFinishLine(lastLat, lastLon, latitude, longitude)) {
            ++lapCounter;
            spdlog::info("Lap crossed! Total laps: {}", lapCounter);
        }
        lastLat = latitude;
        lastLon = longitude;
    }

    // Update the latest distance to the start/finish line (use midpoint for simplicity)
    const double midLat = (startLat1 + startLat2) / 2;
    const double midLon = (startLon1 + startLon2) / 2;
    latestDistance = haversineDistance(latitude, longitude, midLat, midLon);
}

int LapTimer::getNumberOfLaps() const {
    return lapCounter;
}

double LapTimer::getLatestDistance() const {
    return latestDistance;
}

double LapTimer::orientation(const double lat1, const double lon1,
                             const double lat2, const double lon2,
                             const double lat3, const double lon3) {
    return (lat2 - lat1) * (lon3 - lon1) - (lat3 - lat1) * (lon2 - lon1);
}
