#ifndef LOCATION_H
#define LOCATION_H

#include <string>

using namespace std;

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
        float distanceTo(const Location &loc);
        Location sumY(float y) {this->y += y; return *this;}
        string toString();
};

} //end namespace aserver

#endif