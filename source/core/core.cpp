#include <iostream>
#include "core.h"

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
            gen = new generator::Primitive(getPeriodSize());
            break;
        case generator::types::WAVE:
            gen = new generator::Wave(getPeriodSize());
            break;
        case generator::types::TEST:
            gen = new generator::Test(getPeriodSize());
            break;
        case generator::types::SCRIPT:
            //gen = new generator::Script(getPeriodSize());
            break;
        case generator::types::NOISE:
            gen = new generator::Noise(getPeriodSize());
            break;
    }

    gens[generatorCounter++] = gen;

    if (cfgData) {
        gen->config(cfgData);
    }

    return 1;
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
    if (waves.find(filename) != waves.end()) {
        return waves[filename];
    }
    else {
        return nullptr;
    }
}

void Core::shutdown()
{
    if (out) {
        out->close();
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
