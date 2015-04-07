#include "location.h"

#include <cmath>

namespace aserver {

float Location::distanceTo(const Location &loc)
{
    return sqrt((float) pow(this->x - loc.x, 2) + pow(this->y - loc.y, 2) + pow(this->z - loc.z, 2));
}

} //end namespace aserver