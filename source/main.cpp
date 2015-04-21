/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure illustrates the overall architecture.
 * \image latex classes.png "audio-server architecture draft" width=14cm
 *
 */

#include "generator/generator.h"
#include "generator/primitive.h"
#include "generator/test.h"
#include "core.h"
#include "processor/distance_attenuation.h"
#include "generator/wave.h"

using namespace aserver;
using namespace std;


/**
*   \todo "encapsulamento" header files
*/

wavHeader definition and loadWav moved to wav_file.cpp, filename loads in Wave::config, wav_file normalise in progress...

int main () {

    unsigned writePeriods = 100;
    Core core;

    core.setProcessor(processor::types::DISTANCE_ATTENUATION);
    core.setOutput(output::types::FILE);

    //generator::PrimitiveConfigData genData1, genData2, genData3;
    generator::TestConfigData testData1;
    generator::WaveConfigData waveData1;
    processor::DistanceAttenuationSourceConfigData srcData1, srcData2, srcData3;

    // Primitive generator example
    //genData1.frequency = 220;
    //genData1.wft = generator::waveformType::SINE;
    srcData1.loc = Location(0., 5., 0.);
    //core.addGenerator(generator::types::PRIMITIVE, &genData1);
    //core.addSource(&srcData1);

    // Test generator example
    testData1.wft = generator::waveformType::SAWTOOTH;
    srcData2.loc = Location(0., 5., 0.);
    core.addGenerator(generator::types::TEST, &testData1);
    core.addSource();

    // wave generator example
    //srcData3.loc = Location(0., 5., 0.);
    //waveData1.filename = "test.wav"; // input wav files should be placed in the input directory in the project root
    //core.addGenerator(generator::types::WAVE, &waveData1);
    //core.addSource(&srcData3);

    core.render(writePeriods);
}
