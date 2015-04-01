#ifndef LOCATION_H
#define LOCATION_H

namespace aserver {

/** \brief Location contains a 3D space location.
 *
 */

class Location {
    private:
        float x, y, z;

    public:
        Location() : x(0.), y(0.), z(0.) {}
        Location(float x, float y, float z) : x(x), y(y), z(z) {}
        static float distanceBetween2Points(Location x, Location y);
        static float distanceToOrigin(Location x);
};

} //end namespace aserver

#endif