#include <iostream>

#include "core.h"
#include "processor/distance_attenuation.h"
#include "processor/acousticave.h"
#include "processor/no_operation.h"
#include "generator/primitive.h"
#include "generator/test.h"
#include "generator/wave.h"
#include "output/file.h"
#include "output/alsa.h"
#include "output/memory.h"

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
    proc->addSource(it->second, srcData);
}

/** \brief Renders \c n periods to file.
*   If no generators are initialized, the processor will render silence.
*/

void Core::render()
{
    for (auto &gen : gens) {
        gen.second->render();
    }
    proc->render();
    out->write(*proc->buffer);
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

void Core::shutdown()
{
    out->close();
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

void Core::outputConfig(output::OutputConfigData *outputData)
{
    out->config(outputData);
}

} //end namespace aserver
