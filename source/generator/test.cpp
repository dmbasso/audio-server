#include "test.h"

#include <cmath>
#include <iostream>

using namespace std;

namespace aserver {
namespace generator {


    Test::Test(unsigned periodSize) : Primitive(periodSize)
    {
        transitionPeriod = 22050;
        remainingFrames = 0;
        frequencyScaleFactor = 1.05946; //half tone scaling factor
        //frequencyScaleFactor = 1.5; //fifth scaling factor

        distance = 5.;
        angleStep = M_PI/4.;
        initialAngle = M_PI/2.;
        currentAngle = initialAngle;
    }

/** \brief Generates a clockwise rotation for a generator.
 * Default position is along the Y axis (front).
 */

    //\todo computeTestPosition belongs to Test objects.

void Test::render()
{
    unsigned startIndex = 0;

    locs.clear();
    buffer->reset();

    for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
        if (remainingFrames == 0) {
            if ((-2. * M_PI) + initialAngle  - currentAngle == 0) { // one clockwise rotation...
                Primitive::renderNFrames(startIndex, i); //\todo a bit ugly, needs fixing...
                return;
            }

            locs[i] = Location::computeTestPosition(distance, currentAngle);
            Primitive::renderNFrames(startIndex, i);

            cout << "Frequency = " << frequency << " -> " << locs[i].toString() << endl;

            startIndex = i;
            this->currentAngle -= this->angleStep;
            frequency *= frequencyScaleFactor;
            remainingFrames = transitionPeriod;
        }
        remainingFrames--;
    }

    if (startIndex != buffer->getPeriodSize()) {
        Primitive::renderNFrames(startIndex, buffer->getPeriodSize());
    }
}

//\todo correct config for parent classes

void Test::config(const ConfigData *configData)
{
    TestConfigData *cfgData = (TestConfigData*) configData;

    this->transitionPeriod = cfgData->transitionPeriod;
    this->frequencyScaleFactor = cfgData->frequencyScaleFactor;
    this->distance = cfgData->distance;
    this->angleStep = cfgData->angleStep;
    this->initialAngle = cfgData->initialAngle;

    //Primitive::config(configData);
}

} //end generator namespace
} //end aserver namespace
