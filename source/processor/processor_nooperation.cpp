#include "processor_nooperation.h"

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

//\todo loop the Core generators
//\todo check this method with new mixFrame methods

void NoOperation::render()
{
    //\todo we iterate the core generatorss -> update

    this->buffer->reset();

    for (auto const &it : sources) {
        auto gen = it.second->getGenerator();
        gen->render();
    }
}

} //end namespace processor
} //end namespace aserver
