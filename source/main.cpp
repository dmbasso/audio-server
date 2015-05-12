/**
 * This is the main file for the audio-server.
 * It contains methods for running the audio-server main loop, etc.
 * The following figure illustrates the overall architecture.
 * \image latex classes.png "audio-server architecture draft" width=14cm
 *
 */

#include "core/core.h"
#include "temp.h"

using namespace aserver;
using namespace std;

Core core;

// opera data
generator::WaveConfigData celloWaveData, clarinetWaveData, double_bassWaveData, fluteWaveData, harpWaveData, oboeWaveData, percussionWaveData, tromboneWaveData, violinWaveData;
processor::SourceConfigData celloSourceData, clarinetSourceData, double_bassSourceData, fluteSourceData, harpSourceData, oboeSourceData, percussionSourceData,tromboneSourceData, violinSourceData;

void loadOperaData()
{
    celloWaveData.filename = "cello.wav";
    celloSourceData.loc = Location(2., 4., 0.82);
    core.addGenerator(generator::types::WAVE, &celloWaveData);
    core.addSource(&celloSourceData);

    clarinetWaveData.filename = "clarinet.wav";
    clarinetSourceData.loc = Location(-1.45, 6., 0.8);
    core.addGenerator(generator::types::WAVE, &clarinetWaveData);
    core.addSource(&clarinetSourceData);

    double_bassWaveData.filename = "double_bass.wav";
    double_bassSourceData.loc = Location(4.13, 4., 0.86);
    core.addGenerator(generator::types::WAVE, &double_bassWaveData);
    core.addSource(&double_bassSourceData);

    fluteWaveData.filename = "flute.wav";
    fluteSourceData.loc = Location(0., 6., 0.95);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    harpWaveData.filename = "harp.wav";
    harpSourceData.loc = Location(-3.7, 4., 1.08);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    oboeWaveData.filename = "oboe.wav";
    oboeSourceData.loc = Location(2.13, 6., 0.63);
    core.addGenerator(generator::types::WAVE, &oboeWaveData);
    core.addSource(&oboeSourceData);

    percussionWaveData.filename = "percussion.wav";
    percussionSourceData.loc = Location(-3.5, 6., 0.64);
    core.addGenerator(generator::types::WAVE, &percussionWaveData);
    core.addSource(&percussionSourceData);

    tromboneWaveData.filename = "trombone.wav";
    tromboneSourceData.loc = Location(4.2, 6., 1.23);
    core.addGenerator(generator::types::WAVE, &tromboneWaveData);
    core.addSource(&tromboneSourceData);

    violinWaveData.filename = "violin.wav";
    violinSourceData.loc = Location(-1.33, 4., .85);
    core.addGenerator(generator::types::WAVE, &violinWaveData);
    core.addSource(&violinSourceData);
}
/**
*   \todo constructors call config with default data
*   \todo finish all config methods
*   \todo finish all flags
*   \todo use SHRT_MAX instead of 32767...
*   \todo test opera setup with different distance factors
*/

int main () {

    unsigned nPeriods = 100;

/*  processor::ProcessorInput procInput;
    processor::DistanceAttenuationConfigData procData;
    generatePositions();
    procInput.inputListenerPositions = readPositions("data/input/listenerLocations.txt");
    procInput.motionSamplingRate = 240; //Hertz
    procInput.periodSize = 2048;
    procData.input = procInput; */
    core.setProcessor(processor::types::DISTANCE_ATTENUATION);

    core.setOutput(output::types::FILE);

    /*processor::AcousticaveConfigData aaveConfigData;
    aaveConfigData.gain = 1;
    aaveConfigData.hrtf = 1;
    aaveConfigData.modelFilePath = "geometries/model.obj";
    aaveConfigData.reflections = 0;
    aaveConfigData.reverbActive = 0;
    aaveConfigData.area = 3000;
    aaveConfigData.volume = 4000;
    aaveConfigData.rt60 = 5000;
    core.setProcessor(processor::types::ACOUSTICAVE, &aaveConfigData); */

    core.addGenerator(generator::types::PRIMITIVE);
    core.addSource();

    //********** Test generator example
//    generator::TestConfigData testData;
//    testData.flags = generator::primitiveConfigFlags::WAVEFORM;
//    testData.waveform = generator::waveformType::SQUARE;
//    core.addGenerator(generator::types::TEST, &testData);
//    core.addSource();
    
    //********** Noise generator example
//    generator::NoiseConfigData noiseData;
//    noiseData.distType = generator::distributionType::NORMAL;
//    core.addGenerator(generator::types::NOISE, &noiseData);
//    core.addSource();

    //********** Wave generator example
    //generator::WaveConfigData waveData1;
    //processor::SourceConfigData srcData1;
    //srcData1.loc = Location(0., -5., 0.);
    //waveData1.filename = "test.wav"; // input wav files should be placed in the audio/input directory
    //core.addGenerator(generator::types::WAVE, &waveData1);
    //core.addSource(&srcData1);

    //loadOperaData();

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
