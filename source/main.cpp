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

Core core;

// opera data
generator::WaveConfigData celloWaveData, clarinetWaveData, double_bassWaveData, fluteWaveData, harpWaveData, oboeWaveData, percussionWaveData, tromboneWaveData, violinWaveData;
processor::DistanceAttenuationSourceConfigData celloSourceData, clarinetSourceData, double_bassSourceData, fluteSourceData, harpSourceData, oboeSourceData, percussionSourceData,tromboneSourceData, violinSourceData;

void loadOperaData()
{
    celloWaveData.filename = "cello.wav";
    celloWaveData.framesize = 1;
    celloSourceData.loc = Location(2., 4., 0.82);
    core.addGenerator(generator::types::WAVE, &celloWaveData);
    core.addSource(&celloSourceData);

    clarinetWaveData.filename = "clarinet.wav";
    clarinetWaveData.framesize = 1;
    clarinetSourceData.loc = Location(-1.45, 6., 0.8);
    core.addGenerator(generator::types::WAVE, &clarinetWaveData);
    core.addSource(&clarinetSourceData);

    double_bassWaveData.filename = "double_bass.wav";
    double_bassWaveData.framesize = 1;
    double_bassSourceData.loc = Location(4.13, 4., 0.86);
    core.addGenerator(generator::types::WAVE, &double_bassWaveData);
    core.addSource(&double_bassSourceData);

    fluteWaveData.filename = "flute.wav";
    fluteWaveData.framesize = 1;
    fluteSourceData.loc = Location(0., 6., 0.95);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    harpWaveData.filename = "harp.wav";
    harpWaveData.framesize = 1;
    harpSourceData.loc = Location(-3.7, 4., 1.08);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    oboeWaveData.filename = "oboe.wav";
    oboeWaveData.framesize = 1;
    oboeSourceData.loc = Location(2.13, 6., 0.63);
    core.addGenerator(generator::types::WAVE, &oboeWaveData);
    core.addSource(&oboeSourceData);

    percussionWaveData.filename = "percussion.wav";
    percussionWaveData.framesize = 1;
    percussionSourceData.loc = Location(-3.5, 6., 0.64);
    core.addGenerator(generator::types::WAVE, &percussionWaveData);
    core.addSource(&percussionSourceData);

    tromboneWaveData.filename = "trombone.wav";
    tromboneWaveData.framesize = 1;
    tromboneSourceData.loc = Location(4.2, 6., 1.23);
    core.addGenerator(generator::types::WAVE, &tromboneWaveData);
    core.addSource(&tromboneSourceData);

    violinWaveData.filename = "violin.wav";
    violinWaveData.framesize = 1;
    violinSourceData.loc = Location(-1.33, 4., .85);
    core.addGenerator(generator::types::WAVE, &violinWaveData);
    core.addSource(&violinSourceData);
}
/**
*   \todo "encapsulamento" header files
*/

int main () {

    unsigned nPeriods = 2808; //render  +/- 133.4 secs of audio (listener data length)

    processor::ProcessorInput procInput;
    processor::DistanceAttenuationConfigData procData;
    generatePositions();
    procInput.inputListenerPositions = readPositions("data/input/listenerLocations.txt");
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
    //genData1.frequency = 220;
    //genData1.waveform = generator::waveformType::SQUARE;
    //srcData1.loc = Location(-2., 2., 1.5);
    //core.addGenerator(generator::types::PRIMITIVE, &genData1);
    //core.addSource(&srcData1);

    //********** Test generator example
    //testData1.wft = generator::waveformType::SAWTOOTH;
    //srcData2.loc = Location(0., 5., 0.);
    //core.addGenerator(generator::types::TEST, &testData1);
    //core.addSource();

    //********** Wave generator example
    //srcData3.loc = Location(0., 10., 0.);
    //waveData1.filename = "Olson.wav"; // input wav files should be placed in the audio/input directory
    //core.addGenerator(generator::types::WAVE, &waveData1);
    //core.addSource(&srcData3);

    loadOperaData();

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
