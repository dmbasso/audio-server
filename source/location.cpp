#include "location.h"

#include <cmath>

namespace aserver {

float Location::distanceToOrigin(Location x)
{
    return sqrt( (float) pow(x.x,2) + pow(x.y,2) + pow(x.z,2));
}

float Location::distanceBetween2Points(Location x, Location y)
{
    return sqrt( (float) pow(x.x - y.x,2) + pow(x.y - y.y,2) + pow(x.z - y.z,2));
}

} //end namespace aserver