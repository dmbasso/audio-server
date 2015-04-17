#include "no_operation.h"

namespace aserver {
namespace processor {

void NoOperation::config(ConfigData *configData)
{

}

/** \brief Add a Source object to the Processor map.
* This method takes a Generator object to allocate a new Source.
* The new Source object is then added to the processors Source map.
*/
//\todo config data for the no operation processor

void NoOperation::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    auto source = new processor::Source();
    source->setGenerator(gen);
    this->sources[sourceCounter++] = source;
}

void NoOperation::render()
{
    int16_t sams[2];

    this->buffer->reset();

    for (auto const &it : sources) {
        for (int i = 0; i < buffer->getPeriodSize(); i++) {
            it.second->getGenerator()->buffer->readFrame(sams, i); //\todo it.second->getGenerator()->buffer-> instantiate variable outside loop
            this->buffer->mixFrame(sams, i);
        }
    }
}

} //end namespace processor
} //end namespace aserver
