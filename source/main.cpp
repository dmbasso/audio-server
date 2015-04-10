/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure illustrates the overall architecture.
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

    // generate an A3 minor chord: with different note intensities due to their distance from the origin (0,0,0).

    generator::PrimitiveConfigData cfgData1, cfgData2, cfgData3, cfgData4;
    cfgData1.frequency = 220;
    cfgData1.wft = generator::waveformType::SINE;
    cfgData1.location = Location(0., 0., 0.);
    cfgData2.frequency = 262;
    cfgData2.wft = generator::waveformType::SQUARE;
    cfgData2.location = Location(0., 0., 0.);
    cfgData3.frequency = 330;
    cfgData3.wft = generator::waveformType::SAWTOOTH;
    cfgData3.location = Location(0., 0., 0.);

    cfgData4.amplitude = 32767/2;

    //core.addGenerator(generator::types::PRIMITIVE, &cfgData1);
    //core.addSource();

    //core.addGenerator(generator::types::PRIMITIVE, &cfgData2);
    //core.addSource();

    //core.addGenerator(generator::types::PRIMITIVE, &cfgData3);
    //core.addSource();

    core.addGenerator(generator::types::TEST, &cfgData4);
    core.addSource();

    core.render(writePeriods);
}
