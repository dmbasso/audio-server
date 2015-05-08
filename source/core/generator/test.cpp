#include "test.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {


Test::Test(unsigned periodSize) : Primitive(periodSize)
{
    TestConfigData *cfg = new TestConfigData(); //set flags for all data in the constructor
    config(cfg);
    remainingFrames = 0;
    currentAngle = initialAngle;
}

/** \brief Generates a clockwise rotation for a generator.
 * Default position is along the Y axis (front).
 */

void Test::render()
{
    unsigned startIndex = 0;

    locs.clear();
    buffer->reset();

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            if ((-2. * M_PI) + initialAngle  - currentAngle > 0) { // one clockwise rotation...
                return;
            }
            locs[i] = computeTestPosition(distance, currentAngle);
            Primitive::renderNFrames(startIndex, i);

            cout << "Frequency = " << frequency << " -> " << locs[i].toString() << endl;

            startIndex = i;
            currentAngle -= angleStep;
            frequency *= frequencyScaleFactor;
            remainingFrames = transitionPeriod;
        }
        remainingFrames--;
    }

    if (startIndex != buffer->getPeriodSize()) {
        Primitive::renderNFrames(startIndex, buffer->getPeriodSize());
    }
}

void Test::config(const ConfigData *configData)
{
    TestConfigData *cfgData = (TestConfigData*) configData;

    transitionPeriod = cfgData->transitionPeriod;
    frequencyScaleFactor = cfgData->frequencyScaleFactor;
    distance = cfgData->distance;
    angleStep = cfgData->angleStep;
    initialAngle = cfgData->initialAngle;

    Primitive::config(configData);
}

Location Test::computeTestPosition(float distance, float angle)
{
    unsigned decimalCases = 1000000;
    float x = (round(cos(angle) * decimalCases) / decimalCases) * distance;
    float y = (round(sin(angle) * decimalCases) / decimalCases) * distance;

    return Location(x, y, 0.);
}

} //end generator namespace
} //end aserver namespace
