/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure illustrates the overall architecture.
 * \image latex classes.png "audio-server architecture draft" width=14cm
 *
 */

#include "generator/generator.h"
#include "generator/generator_primitive.h"
#include "core.h"
#include "processor/processor_distance_attenuation.h"

using namespace aserver;
using namespace std;

int main () {

    unsigned writePeriods = 100;
    Core core;

    core.setProcessor(processor::types::DISTANCE_ATTENUATION);
    core.setOutput(output::types::FILE);

    // generate an A3 minor chord: with different note intensities due to their distance from the origin (0,0,0).

    generator::PrimitiveConfigData genData1, genData2, genData3;
    processor::DistanceAttenuationSourceConfigData srcData1, srcData2, srcData3, srcData4;

    genData1.frequency = 220;
    genData1.wft = generator::waveformType::SINE;
    srcData1.loc = Location(0., 5., 0.);
    //core.addGenerator(generator::types::PRIMITIVE, &genData1);
    //core.addSource(&srcData1);
    
    genData2.frequency = 262;
    genData2.wft = generator::waveformType::SQUARE;
    srcData2.loc = Location(0., 8., 0.);
    //core.addGenerator(generator::types::PRIMITIVE, &genData2);
    //core.addSource(&srcData2);

    genData3.frequency = 330;
    genData3.wft = generator::waveformType::SAWTOOTH;
    srcData3.loc = Location(0., 10., 0.);
    //core.addGenerator(generator::types::PRIMITIVE, &genData3);
    //core.addSource(&srcData3);

    //srcData4.loc = Location(0., 20., 0.);
    core.addGenerator(generator::types::TEST);
    core.addSource();

    core.render(writePeriods);
}
