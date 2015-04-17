#include "acousticave.h"

namespace aserver {
namespace processor {

void Acousticave::config(ConfigData *configData)
{
    //auto cfg = (AcousticaveConfigData*) configData;
    //if (cfg->flags & blabla::setReverb) {
    //    bla bla bla
    //}
}

void Acousticave::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    //auto source = new processor::AcousticaveSource
    //insert it
}

void Acousticave::render()
{
    //do whatever Acousticave needs to be done,
    //then copy the results into `buffer`
}

} //end namespace processor
} //end namespace aserver