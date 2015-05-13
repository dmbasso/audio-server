#include "no_operation.h"

namespace aserver {
namespace processor {

/** \brief Add a Source object to the Processor map.
* This method takes a Generator object to allocate a new Source.
* The new Source object is then added to the processors Source map.
*/
//\todo Source parent implements noOperationSource processor

void NoOperation::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    auto source = new processor::Source();
    source->setGenerator(gen);

    if (srcData) {
        source->setLocation(srcData->loc);
    }
    sources[sourceCounter++] = source;
}

void NoOperation::render()
{
    int16_t sams[2];

    buffer->reset();

    for (auto const &it : sources) {
        SoundBuffer *sb = it.second->getGenerator()->buffer;
        for (int i = 0; i < buffer->getPeriodSize(); i++) {
            sb->readFrame(sams, i);
            buffer->mixFrame(sams, i);
        }
    }
}

} //end namespace processor
} //end namespace aserver
