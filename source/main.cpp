/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure ilustrates the overall architecture.
 * \image latex classes.png "audio-server architecture draft" width=14cm
 *
 */

#include "generator.h"
#include "core.h"

using namespace aserver;
using namespace std;

int main () {

    unsigned writePeriods = 100;
    Core core;

    core.setProcessor(processor::types::DISTANCE_ATTENUATION);

    core.setOutput(output::types::FILE);

    // generate an A3 minor chord with diferent note intensities due to their distance from the origin (0,0,0).
    generator::ConfigData *cfgData1 = new generator::PrimitiveConfigData();
    ((generator::PrimitiveConfigData*) cfgData1)->setFrequency(220);
    ((generator::PrimitiveConfigData*) cfgData1)->setLocation(Location(0., 0., 0.));
    generator::ConfigData *cfgData2 = new generator::PrimitiveConfigData();
    ((generator::PrimitiveConfigData*) cfgData2)->setFrequency(262);
    ((generator::PrimitiveConfigData*) cfgData2)->setLocation(Location(0., 0., 0.));
    generator::ConfigData *cfgData3 = new generator::PrimitiveConfigData();
    ((generator::PrimitiveConfigData*) cfgData3)->setFrequency(330);
    ((generator::PrimitiveConfigData*) cfgData3)->setLocation(Location(0., 0., 0.));

    core.addGenerator(generator::types::PRIMITIVE, cfgData1);
    core.addSource();

    core.addGenerator(generator::types::PRIMITIVE, cfgData2);
    core.addSource();

    core.addGenerator(generator::types::PRIMITIVE, cfgData3);
    core.addSource();

    core.render(writePeriods);

    return 1;
}
