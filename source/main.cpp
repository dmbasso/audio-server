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
    celloWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(celloWaveData.filename, "audio/input/cello.wav", 256);
    celloSourceData.loc = Location(2., 4., 0.82);
    core.addGenerator(generator::types::WAVE, &celloWaveData);
    core.addSource(&celloSourceData);

    clarinetWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(clarinetWaveData.filename, "audio/input/clarinet.wav", 256);
    clarinetSourceData.loc = Location(-1.45, 6., 0.8);
    core.addGenerator(generator::types::WAVE, &clarinetWaveData);
    core.addSource(&clarinetSourceData);

    double_bassWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(double_bassWaveData.filename, "audio/input/double_bass.wav", 256);
    double_bassSourceData.loc = Location(4.13, 4., 0.86);
    core.addGenerator(generator::types::WAVE, &double_bassWaveData);
    core.addSource(&double_bassSourceData);

    fluteWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(fluteWaveData.filename, "audio/input/flute.wav", 256);
    fluteSourceData.loc = Location(0., 6., 0.95);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    harpWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(harpWaveData.filename, "audio/input/harp.wav", 256);
    harpSourceData.loc = Location(-3.7, 4., 1.08);
    core.addGenerator(generator::types::WAVE, &fluteWaveData);
    core.addSource(&fluteSourceData);

    oboeWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(oboeWaveData.filename, "audio/input/oboe.wav", 256);
    oboeSourceData.loc = Location(2.13, 6., 0.63);
    core.addGenerator(generator::types::WAVE, &oboeWaveData);
    core.addSource(&oboeSourceData);

    percussionWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(percussionWaveData.filename, "audio/input/percussion.wav", 256);
    percussionSourceData.loc = Location(-3.5, 6., 0.64);
    core.addGenerator(generator::types::WAVE, &percussionWaveData);
    core.addSource(&percussionSourceData);

    tromboneWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(tromboneWaveData.filename, "audio/input/trombone.wav", 256);
    tromboneSourceData.loc = Location(4.2, 6., 1.23);
    core.addGenerator(generator::types::WAVE, &tromboneWaveData);
    core.addSource(&tromboneSourceData);

    violinWaveData.flags = generator::waveConfigFlags::WAVE_ALL;
    strncpy(violinWaveData.filename, "audio/input/violin.wav", 256);
    violinSourceData.loc = Location(-1.33, 4., .85);
    core.addGenerator(generator::types::WAVE, &violinWaveData);
    core.addSource(&violinSourceData);
}
/**
*   \todo processor listener positions in config (2 bytes for processor base flags?)
*   \todo test opera setup with different distance factors
*/

int main () {

    unsigned nPeriods = 3700;

    //********** No Operation processor example
//    core.setProcessor(processor::types::NO_OPERATION);

    //********** Distance Attenuation processor example
    processor::ProcessorInput procInput;
    generatePositions();
    procInput.inputListenerPositions = readPositions("data/input/listenerLocations.txt");
    procInput.motionSamplingRate = 240; //Hertz
    procInput.periodSize = 2048;
    processor::DistanceAttenuationConfigData procData;
    procData.flags = processor::distanceAttenuationConfigFlags::DISTANCEATTENUATION_ALL;
    procData.distanceExp = 1;
    procData.withInputPositions = true;
    procData.input = procInput;
    core.setProcessor(processor::types::DISTANCE_ATTENUATION, &procData);

    //********** Acousticave processor example
//    processor::AcousticaveConfigData aaveConfigData;
//    aaveConfigData.flags = processor::acousticaveConfigFlags::ACOUSTICAVE_ALL;
//    aaveConfigData.gain = 1;
//    aaveConfigData.hrtf = 4;
//    aaveConfigData.modelFilePath = "geometries/model.obj";
//    aaveConfigData.reflections = 0;
//    aaveConfigData.reverbActive = 1;
//    aaveConfigData.area = 3000;
//    aaveConfigData.volume = 3000;
//    aaveConfigData.rt60 = 2000;
//    core.setProcessor(processor::types::ACOUSTICAVE, &aaveConfigData);

    //********** Memory output example
//    core.setOutput(output::types::MEMORY);

    //********** File output example
    output::FileOutputConfigData fileData;
    fileData.flags = output::fileConfigFlags::NORMALISE;
    fileData.normalise_audio = true;
    core.setOutput(output::types::FILE, &fileData);

    //********** Alsa output example
//    output::AlsaOutputConfigData alsaData;
//    alsaData.flags = output::alsaConfigFlags::ALSA_ALL;
//    alsaData.samplingRate = 44100;
//    alsaData.nChannels = 2;
//    alsaData.withPulseAudio = true;
//    core.setOutput(output::types::ALSA, &alsaData);

    //********** Primitive generator example
//    core.addGenerator(generator::types::PRIMITIVE);
//    core.addSource();

    //********** Test generator example
//    generator::TestConfigData testData;
//    testData.flags = generator::primitiveConfigFlags::WAVEFORM;
//    testData.waveform = generator::waveformType::SQUARE;
//    core.addGenerator(generator::types::TEST, &testData);
//    core.addSource();
    
    //********** Noise generator example
//    generator::NoiseConfigData noiseData;
//    noiseData.flags = generator::noiseConfigFlags::NOISE_ALL;
//    noiseData.amplitude = SHRT_MAX * 0.02;
//    noiseData.distType = generator::distributionType::NORMAL;
//    core.addGenerator(generator::types::NOISE, &noiseData);
//    core.addSource();

    //********** Wave generator example
//    generator::WaveConfigData waveData;
//    waveData.flags = generator::waveConfigFlags::WAVE_ALL;
//    strncpy(waveData.filename, "audio/input/espiral.wav", 256); // input wav filepath must be fully set
//    waveData.increment = 1.;
//    waveData.position = 0;
//    processor::SourceConfigData srcData1;
//    srcData1.loc = Location(0., 5., 0.);
//    core.addGenerator(generator::types::WAVE, &waveData);
//    core.addSource(&srcData1);

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
