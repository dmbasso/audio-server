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

#include "temp.h"

using namespace aserver;
using namespace std;


/**
*   \todo "encapsulamento" header files
*/

int main () {

    unsigned nPeriods = 430; //render  +/- 20 secs of audio
    Core core;

    processor::ProcessorInput procInput;
    processor::DistanceAttenuationConfigData procData;
    generatePositions();
    procInput.inputListenerPositions = readPositions();
    procInput.motionSamplingRate = 240; //Hertz
    procInput.periodSize = 2048;
    procData.input = procInput;
    core.setProcessor(processor::types::DISTANCE_ATTENUATION, &procData);

    core.setOutput(output::types::FILE);

    generator::PrimitiveConfigData genData1;
    generator::TestConfigData testData1;
    generator::WaveConfigData waveData1;
    processor::DistanceAttenuationSourceConfigData srcData1, srcData2, srcData3;

    //********** Primitive generator example
    genData1.frequency = 220;
    genData1.waveform = generator::waveformType::SQUARE;
    srcData1.loc = Location(0., 10., 0.);
    core.addGenerator(generator::types::PRIMITIVE, &genData1);
    core.addSource(&srcData1);

    //********** Test generator example
    //testData1.wft = generator::waveformType::SAWTOOTH;
    //srcData2.loc = Location(0., 5., 0.);
    //core.addGenerator(generator::types::TEST, &testData1);
    //core.addSource();

    //********** Wave generator example
    //srcData3.loc = Location(0., 10., 0.);
    //waveData1.filename = "Olson.wav"; // input wav files should be placed in the input directory in the project root
    //core.addGenerator(generator::types::WAVE, &waveData1);
    //core.addSource(&srcData3);

    for (int i = 0; i < nPeriods; i++) {
        core.render();
    }

    core.shutdown();
}

/*
float f = 0;
for (int i=0, vector::iterator it=p.begin(); it!=p.end();)  {
while (it!=p.end() && i * 1. / 240 < f + 2048. / 44100) {
core.processor->listenerPositions[(int)((i * 1. / 240 - f) * 44100)] = *it;
++it; ++i;
}
f += 2048. / 44100;
core.render();
}*/
