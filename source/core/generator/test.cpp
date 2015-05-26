#include "test.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {


Test::Test(Core *core, uint32_t periodSize) : Primitive(core, periodSize)
{
    TestConfigData *cfgData = new TestConfigData(); // set flags for all data in the constructor
    cfgData->flags = testConfigFlags::TEST_ALL; // configure all data fields in constructor
    config(cfgData);
    currentAngle = initialAngle;
}

void Test::config(const ConfigData *configData)
{
    TestConfigData *cfgData = (TestConfigData*) configData;

    Primitive::config(configData);

    if (cfgData->flags & testConfigFlags::TRANSITION_PERIOD) {
        transitionPeriod = cfgData->transitionPeriod;
    }
    if (cfgData->flags & testConfigFlags::FREQUENCY_SCALE_FACTOR) {
        frequencyScaleFactor = cfgData->frequencyScaleFactor;
    }
    if (cfgData->flags & testConfigFlags::DISTANCE) {
        distance = cfgData->distance;
    }
    if (cfgData->flags & testConfigFlags::ANGLE_STEP) {
        angleStep = cfgData->angleStep;
    }
    if (cfgData->flags & testConfigFlags::INITIAL_ANGLE) {
        initialAngle = cfgData->initialAngle;
    }
}

/** \brief Generates a clockwise rotation for a generator.
 * Default position is along the Y axis (front).
 */

void Test::render()
{
    uint32_t startIndex = 0;

    locs.clear();
    buffer->reset();

    for (uint32_t i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            if ((-2. * M_PI) + initialAngle  - currentAngle > 0) { // one clockwise rotation...
                return;
            }
            locs[i] = computeTestPosition(distance, currentAngle);
            Primitive::renderNFrames(startIndex, i);

            cout << "Frequency = " << frequency << " Hz -> " << locs[i].toString() << endl;

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

Location Test::computeTestPosition(float distance, float angle)
{
    uint32_t decimalCases = 1000000;
    float x = (round(cos(angle) * decimalCases) / decimalCases) * distance;
    float y = (round(sin(angle) * decimalCases) / decimalCases) * distance;

    return Location(x, y, 0.);
}

} //end generator namespace
} //end aserver namespace
