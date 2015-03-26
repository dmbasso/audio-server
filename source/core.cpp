#include "core.h"

namespace aserver {

/** \brief Note: This method takes as input a specific ConfigData for the generator to be instantiated.
*
*/

int Core::addGenerator(generatorType genType, generator::ConfigData *cfgdata)
{
    switch (genType) {
        case generatorType::PRIMITIVE: {
            generator::Generator* prim = new generator::Primitive(getPeriodSize());
            if (cfgdata)
                prim->config(cfgdata);
            generators.insert(std::pair<int, generator::Generator*>(generator_counter++,prim));
            break;
        }
        case generatorType::WAVE:
            break;
        case generatorType::TEST:
            break;
        case generatorType::SCRIPT:
            break;
    }
    return 1;
}

int Core::setProcessor(processorType procType)
{
    switch (procType) {
        case processorType::NOOPERATION:
            processor = new processor::NoOperation(this->period_size);
            break;
        case processorType::ACOUSTICAVE:
            processor = new processor::Acousticave(this->period_size);
            break;
        case processorType::DISTANCEATTENUATION:
            processor = new processor::DistanceAttenuation(this->period_size);
            break;
    }
    return 1;
}

int Core::setOutput(outputType outType)
{
    switch(outType) {
        case outputType::FILE:
            output = new output::File();
            break;
        case outputType::ALSA:
            output = new output::Alsa();
            break;
        case outputType::MEMORY:
            output = new output::Memory();
            break;
    }
}

/** \brief Get the last generator added and place it in the processor source map.
*
*/

int Core::addSource()
{
    std::map<int, generator::Generator*>::iterator it = --generators.end();
    this->processor->addSource(it->second);
}

/** \brief Renders \c n periods to file.
*
*/

void Core::renderFile(unsigned n)
{
    for (int i = 0; i < n; i++) {
        processor->render();
        output->write(*processor->buffer);
    }
    output->close();
}

} //end namespace aserver
