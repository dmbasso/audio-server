#include "location.h"

#include <cmath>

namespace aserver {

float Location::distanceTo(const Location &loc)
{
    return sqrt(pow(x - loc.x, 2) + pow(y - loc.y, 2) + pow(z - loc.z, 2));
}

string Location::toString()
{
    return "Location = " + to_string(x) + ' ' + to_string(y) + ' ' + to_string(z);
}

} //end namespace aserver