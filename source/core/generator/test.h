#ifndef GENERATOR_TEST_H
#define GENERATOR_TEST_H

#include "primitive.h"

using namespace std;

namespace aserver {
namespace generator {

enum testConfigFlags : uint64_t {
    TRANSITION_PERIOD       = 0x10000,
    FREQUENCY_SCALE_FACTOR  = 0x20000,
    DISTANCE                = 0x40000,
    ANGLE_STEP              = 0x80000,
    INITIAL_ANGLE          = 0x100000,
    TEST_ALL               = 0x1F0000
};

/** \brief Test configuration for the signal primitives generated.
*   Bit flags for configuring data: (PrimitiveConfigData flags) + transitionPeriod + frequencyScaleFactor + distance + angleStep + initialAngle
*/

struct TestConfigData : PrimitiveConfigData {
    unsigned transitionPeriod = 44100;
    float frequencyScaleFactor = 1.05946; //half tone scaling factor
    double distance = 10.;
    double angleStep = M_PI/4.;
    double initialAngle = M_PI/2.;
};

/**
 * Contains the script instructions for altering the properties of the primitive during playback.
 * This script is a map with key=frame  and Entry=Location
 */

class Test :public Primitive {
private:
    unsigned transitionPeriod;
    unsigned remainingFrames = 0;
    float frequencyScaleFactor;

    double distance; // overrides source location...
    double angleStep;
    double initialAngle;
    double currentAngle;

public:
    Test(Core *core, unsigned periodSize);
    void config(const ConfigData *configData) override;
    virtual void render() override;
    Location computeTestPosition(float distance, float angle);
};

} //end generator namespace
} //end aserver namespace

#endif
