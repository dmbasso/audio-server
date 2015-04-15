#include "location.h"

#include <cmath>

namespace aserver {

float Location::distanceTo(const Location &loc)
{
    return sqrt(pow(this->x - loc.x, 2) + pow(this->y - loc.y, 2) + pow(this->z - loc.z, 2));
}

string Location::toString()
{
    return "Location = " + to_string(x) + ' ' + to_string(y) + ' ' + to_string(z);
}

Location Location::computeTestPosition(float distance, float angle)
{
    unsigned decimalCases = 1000000;
    float x = (round(cos(angle) * decimalCases) / decimalCases) * distance;
    float y = (round(sin(angle) * decimalCases) / decimalCases) * distance;

    return Location(x, y, 0.);
}

} //end namespace aserver