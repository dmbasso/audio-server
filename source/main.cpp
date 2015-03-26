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

    unsigned write_periods = 100;
    Core core;

    core.setProcessor(processorType::DISTANCEATTENUATION);

    core.setOutput(outputType::FILE);

    // generate an A2 minor chord with diferent note intensities due to their distance from the origin (0,0,0).

    generator::ConfigData *cfgdata1 = new generator::PrimitiveConfigData(32767, 0, 220, 2, Location(0., 10., 0.));
    generator::ConfigData *cfgdata2 = new generator::PrimitiveConfigData(32767, 0, 262, 2, Location(0., 5., 0.));
    generator::ConfigData *cfgdata3 = new generator::PrimitiveConfigData(32767, 0, 330, 2, Location(0., 15., 0.));

    core.addGenerator(generatorType::PRIMITIVE, cfgdata1);
    core.addSource();

    core.addGenerator(generatorType::PRIMITIVE, cfgdata2);
    core.addSource();

    core.addGenerator(generatorType::PRIMITIVE, cfgdata3);
    core.addSource();

    core.renderFile(write_periods);

    return 1;
}
