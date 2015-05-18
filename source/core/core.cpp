#include <iostream>
#include "core.h"
#include "wav_file.h"

using namespace std;

namespace aserver {

/** \brief Note: This method takes as input a specific ConfigData for the generator to be instantiated.
*  \todo When does the core::waves map load the wave soundBuffer?
*/

int Core::addGenerator(generator::types genType, generator::ConfigData *cfgData)
{
    generator::Generator *gen;
    switch (genType) {
        case generator::types::PRIMITIVE:
            gen = new generator::Primitive(this, getPeriodSize());
            break;
        case generator::types::WAVE:
            gen = new generator::Wave(this, getPeriodSize());
            break;
        case generator::types::TEST:
            gen = new generator::Test(this, getPeriodSize());
            break;
        case generator::types::SCRIPT:
            gen = new generator::Script(this, getPeriodSize());
            break;
        case generator::types::NOISE:
            gen = new generator::Noise(this, getPeriodSize());
            break;
    }
    int gid = generatorCounter++;
    gens[gid] = gen;
    if (cfgData) {
        gen->config(cfgData);
    }
    return gid;
}

int Core::setProcessor(processor::types procType, processor::ConfigData *cfgData)
{
    switch (procType) {
        case processor::types::NO_OPERATION:
            proc = new processor::NoOperation(periodSize);
            break;
        case processor::types::ACOUSTICAVE:
            proc = new processor::Acousticave(periodSize);
            break;
        case processor::types::DISTANCE_ATTENUATION:
            proc = new processor::DistanceAttenuation(periodSize);
            break;
    }

    if (cfgData) {
        proc->config(cfgData);
    }
    return 1;
}

int Core::setOutput(output::types outType, output::ConfigData *cfgData)
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
    if (cfgData) {
        out->config(cfgData);
    }
    return 1;
}

/** \brief Get the last generator added and place it in the processor source map.
*
*/

int Core::addSource(processor::SourceConfigData *srcData)
{
    if (proc) {
        map<int, generator::Generator*>::reverse_iterator it = gens.rbegin();
        // TODO: return the source id
        // return proc->addSource(it->second, srcData);
        proc->addSource(it->second, srcData);
    }
    return -1;
}

/** \brief Renders \c n periods to file.
*   If no generators are initialized, the processor will render silence.
*/

void Core::render()
{
    if (!proc || !out) {
        return;
    }
    for (auto &gen : gens) {
        gen.second->render();
    }
    proc->render();
    out->write(*proc->buffer);
}

SoundBuffer* Core::getWave(const char *filename)
{
    for (auto waveIt : waves) {
        if (!strcmp(waveIt.first, filename)) {
            return waveIt.second;
        }
    }
    return waves[filename] = loadWave(filename);
}

void Core::stop_output()
{
    if (out) {
        out->close();
    }
}

uint64_t Core::get_output(int16_t **dest)
{
    if (!out) {
        return 0;
    }
    if (output::Memory *mem = dynamic_cast<output::Memory*>(out)) {
        return mem->get_output(dest);
    }
    return 0;
}

void Core::shutdown()
{
    stop_output();
    // todo: shutdown the rest of the system
    if (out) {
        delete out;
        out = nullptr;
    }
}

void Core::generatorConfig(int gid, generator::ConfigData *configData)
{
    if (gens.find(gid) != gens.end()) {
        gens[gid]->config(configData);
    }
    else {
        cout << "Generator " << gid << " is not allocated..." << endl;
    }
}

void Core::processorConfig(processor::ConfigData *configData)
{
    proc->config(configData);
}
void Core::sourceConfig(int sid, processor::SourceConfigData *srcData)
{
    if (proc->sources.find(sid) != proc->sources.end()) {
        proc->sources[sid]->config(srcData);
    }
}

void Core::outputConfig(output::ConfigData *outputData)
{
    out->config(outputData);
}

} //end namespace aserver
