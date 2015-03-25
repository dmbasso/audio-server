#include "core.h"

namespace aserver {

int Core::addGenerator(generatorType genType)
{
    switch (genType) {
        case generatorType::PRIMITIVE: {
            generator::Generator* prim = new generator::Primitive(getPeriodSize());
            generators.insert(std::pair<int, generator::Generator*>(generator_counter,prim));
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

int Core::addSource()
{
    std::map<int, generator::Generator*>::iterator it = --generators.end();
    this->processor->addSource(it->second);
}

void Core::render(unsigned write_periods)
{
    for (int i = 0; i < write_periods; i++) {
        processor->render();
        output->write(*processor->buffer);
    }
    output->close();
}

} //end namespace aserver
