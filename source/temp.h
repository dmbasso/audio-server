#ifndef TEMP_H
#define TEMP_H

#include <vector>
#include "core/location.h"

void generatePositions();
vector<aserver::Location> readPositions(const string filename);

#endif
