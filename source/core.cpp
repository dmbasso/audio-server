#include <iostream>
#include <fstream>

#include "core.h"
#include "processor/distance_attenuation.h"
#include "processor/acousticave.h"
#include "processor/no_operation.h"
#include "generator/primitive.h"
#include "generator/test.h"
#include "generator/wave.h"
#include "wav_header.h"

using namespace std;

namespace aserver {

/** \brief Note: This method takes as input a specific ConfigData for the generator to be instantiated.
* Currently, in the wave generator, it includes the last wave read into the waves map.
*/

int Core::addGenerator(generator::types genType, generator::ConfigData *cfgdata)
{
    generator::Generator *gen;

    switch (genType) {
        case generator::types::PRIMITIVE:
            gen = new generator::Primitive(getPeriodSize());
            break;
        case generator::types::WAVE: {
            map<std::string, SoundBuffer *>::reverse_iterator it = waves.rbegin(); //\todo string filename as config data wave
            gen = new generator::Wave(getPeriodSize(), it->second); //\todo buffer is loaded upon configuration, not init.
            break;
        }
        case generator::types::TEST:
            gen = new generator::Test(getPeriodSize());
            break;
        case generator::types::SCRIPT:
            //gen = new generator::Script(getPeriodSize());
            break;
    }

    gens[generatorCounter++] = gen;

    if (cfgdata) {
        gen->config(cfgdata);
    }

    return 1;
}

int Core::setProcessor(processor::types procType)
{
    switch (procType) {
        case processor::types::NO_OPERATION:
            proc = new processor::NoOperation(this->periodSize);
            break;
        case processor::types::ACOUSTICAVE:
            proc = new processor::Acousticave(this->periodSize);
            break;
        case processor::types::DISTANCE_ATTENUATION:
            proc = new processor::DistanceAttenuation(this->periodSize);
            break;
    }
    return 1;
}

int Core::setOutput(output::types outType)
{
    switch(outType) {
        case output::types::FILE:
            out = new output::File();
            break;
        case output::types::ALSA:
            out = new output::Alsa();
            break;
        case output::types::MEMORY:
            out = new output::Memory();
            break;
    }
}

/** \brief Get the last generator added and place it in the processor source map.
*
*/

int Core::addSource(processor::SourceConfigData *srcData)
{
    map<int, generator::Generator*>::reverse_iterator it = gens.rbegin();
    this->proc->addSource(it->second, srcData);
}

/** \brief Renders \c n periods to file.
*   If no generators are initialized, the processor will render silence.
*/

void Core::render(unsigned writePeriods)
{
    for (int i = 0; i < writePeriods; i++) {
        for (auto &gen : gens) {
            gen.second->render();
        }
        proc->render();
        out->write(*proc->buffer);
    }
    out->close();
}
    //\todo wav_file.loadWave takes care of reading the sound file
    //\todo merge read and get wave, to manage wave buffers: readWave just searches the waves map, returns buffer pointer or nullptr

int Core::readWave(const string filename)
{
    ifstream ifs;
    string filePath = "input/" + filename;

    ifs.open(filePath, ifstream::in);
    if (ifs.is_open()) {
        wavHeader wh;
        ifs.read(reinterpret_cast<char *>(&wh), sizeof(wavHeader));
        cout << "\nReading wavfile = " << filename << "\nDatachunkSize = " << wh.datachunkSize;
        cout << " numChannels = " << wh.numChannels << " Soundbuffer period size = " << wh.datachunkSize/ (wh.numChannels*2) << endl << endl;

        SoundBuffer *sb = new SoundBuffer(wh.datachunkSize/ (wh.numChannels*2), wh.numChannels);
        ifs.read(reinterpret_cast<char *>(sb->getData()), wh.datachunkSize);
        waves[filename] = sb;
        return 1;
    }
    else {
        cout << "\nError opening file " << filename << endl;
        return 0;
    }
}

SoundBuffer* Core::getWave(const string filename)
{
    if (waves.find(filename) != waves.end()) {
        return waves[filename];
    }
    else {
        return nullptr;
    }
}

} //end namespace aserver
