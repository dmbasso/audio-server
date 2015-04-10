#include "core.h"

namespace aserver {

/** \brief Note: This method takes as input a specific ConfigData for the generator to be instantiated.
*
*/

int Core::addGenerator(generator::types genType, generator::ConfigData *cfgdata)
{
    generator::Generator *gen;

    switch (genType) {
        case generator::types::PRIMITIVE: {
            gen = new generator::Primitive(getPeriodSize());
            break;
        }
        case generator::types::WAVE:
            break;
        case generator::types::TEST:
            gen = new generator::Test(getPeriodSize());
            break;
        case generator::types::SCRIPT:
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

int Core::addSource()
{
    std::map<int, generator::Generator*>::reverse_iterator it = gens.rbegin();
    this->proc->addSource(it->second);
}

/** \brief Renders \c n periods to file.
*   //\todo check for empty generator map and empty source map
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

} //end namespace aserver
