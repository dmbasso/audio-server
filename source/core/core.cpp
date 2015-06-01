#include <iostream>
#include "core.h"
#include "wav_file.h"

using namespace std;

namespace aserver {

/** \brief Note: This method takes as input a specific ConfigData for the generator to be instantiated.
*
*/

int16_t Core::addGenerator(generator::types genType, generator::ConfigData *cfgData)
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
        default:
            return -1;
    }

    int16_t gid = generatorCounter++;
    gens[gid] = gen;

    if (cfgData) {
        gen->config(cfgData);
    }
    return gid;

}

int Core::setProcessor(processor::types procType, processor::ConfigData *cfgData)
{
    map<int32_t, processor::Source *> tempSources;
    // in case we are changing processor, store the existing sources
    if (proc) {
        tempSources = proc->sources;
    }

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

    // in case we are changing processor, load the existing sources
    if (!tempSources.empty()) {
        proc->sources = tempSources;
        if (procType == processor::types::ACOUSTICAVE) {
            for (auto it : tempSources) {
                // add sources to the new aave instance
                ((processor::Acousticave *) proc)->addAaveSource(it.second);
            }
        }
    }
    return 1;
}

int Core::setOutput(output::types outType, output::ConfigData *cfgData)
{
    switch(outType) {
        case output::types::FILE:
            out = new output::File(this, getPeriodSize());
            break;
        case output::types::ALSA:
            out = new output::Alsa(this, getPeriodSize());
            break;
        case output::types::MEMORY:
            out = new output::Memory(this, getPeriodSize());
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

int16_t Core::addSource(processor::SourceConfigData *srcData)
{
    if (proc) {
        if (!gens.empty()) {
            map<int32_t, generator::Generator *>::reverse_iterator it = gens.rbegin();
            return proc->addSource(it->second, srcData);
        }
        else {
            return proc->addSource(nullptr, srcData);
        }
    }
    return -1;
}


void Core::setSourceGenerator(int16_t sid, int16_t gid)
{
    proc->sources[sid]->setGenerator(gens[gid]);
}

/** \brief Renders \c n periods to file.
*   If no generators are initialized, the processor will render silence.
*/

void Core::render()
{
    if (!out) {
        return;
    }

    for (auto &gen : gens) {
        gen.second->render();
    }

    if (proc) {
        proc->render();
    }
    if (out) {
        if (proc) {
            out->write(proc->buffer);
        }
        else {
            out->write(nullptr);
        }
    }
}

SoundBuffer* Core::getWave(const char *filename)
{
    for (auto waveIt : waves) {
        if (!waveIt.first.compare(filename)) {
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
    reset();
}

void Core::reset(bool clearOutput)
{
    for (auto it : gens) {
        delete it.second;
    }
    gens.clear();
    generatorCounter = 0;

    for (auto it : waves) {
        delete it.second;
    }
    waves.clear();

    if (proc) {
        delete proc;
        proc = nullptr;
    }

    if (out && clearOutput) {
        out->close();
        delete out;
        out = nullptr;
    }
}

void Core::generatorConfig(int32_t gid, generator::ConfigData *configData)
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
void Core::sourceConfig(int32_t sid, processor::SourceConfigData *srcData)
{
    if (proc->sources.find(sid) != proc->sources.end()) {
        proc->sources[sid]->config(srcData);
    }
}

void Core::outputConfig(output::ConfigData *outputData)
{
    out->config(outputData);
}

void Core::setPeriodSize(uint32_t periodSize)
{
    this->periodSize = periodSize;

    for (auto gen : gens) {
        gen.second->setPeriodSize(periodSize);
    }

    if (proc) {
        proc->setPeriodSize(periodSize);
    }

    if (out) {
        out->setPeriodSize(periodSize);
    }
}

} //end namespace aserver
