#include <cmath>

#include "processor.h"
#include "location.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
}

void Processor::addBufferData(SoundBuffer *buf)
{
    int16_t *tempBufferProc = this->buffer->read();
    int16_t *tempBuffer = buf->read();
    for (int i = 0; i < this->buffer->getDataSize(); i++) {
        tempBufferProc[i] += tempBuffer[i];
    }
}

void NoOperation::config(ConfigData *configData)
{

}

/** \brief Add a Source object to the Processor map.
 * This method takes a Generator object to allocate a new Source.
 * The new Source object is then added to the processors Source map.
 */

void NoOperation::addSource(generator::Generator *gen)
{
        auto source = new processor::Source();
        source->setGenerator(gen);
        source->setLocation(gen->getLocation());
        this->sources.insert(std::pair<int, Source*>(sourceCounter++,source));
}

void NoOperation::render()
{
    //Should we iterate the generators in the processors source map or the core generators?
    //Currently iterating the processor sources..

    this->buffer->reset();

    for (auto const &it : sources) {
        auto gen = it.second->getGenerator();
        gen->render();
        addBufferData(gen->buffer);
    }
}

void Acousticave::config(ConfigData *configData)
{
    //auto cfg = (AcousticaveConfigData*) configData;
    //if (cfg->flags & blabla::setReverb) {
    //    bla bla bla
    //}
}

void Acousticave::addSource(generator::Generator *gen)
{
    //auto source = new processor::AcousticaveSource
    //insert it
}

void Acousticave::render()
{
    //do whatever Acousticave needs to be done,
    //then copy the results into `buffer`
}

void DistanceAttenuation::config(ConfigData *configData)
{
    //auto cfg = (AcousticaveConfigData*) configData;
    //if (cfg->flags & blabla::setReverb) {
    //    bla bla bla
    //}
}

/** \brief Add a Source object to the Processor map.
* This method takes a Generator object to allocate a new Source.
* The new Source object is then added to the processors Source map.
*/

void DistanceAttenuation::addSource(generator::Generator *gen)
{
    auto source = new processor::Source();
    source->setGenerator(gen);
    source->setLocation(gen->getLocation());
    this->sources.insert(std::pair<int, Source*>(sourceCounter++,source));
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::render()
{
    //Should we iterate the generators in the processors source map or the core generators?
    //Currently iterating the processor sources...

    this->buffer->reset();

    for (auto const &it : sources) {
        auto gen = it.second->getGenerator();
        gen->render();
        process(gen);
        addBufferData(gen->buffer);
    }
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the scalar with every sample in the source rendered buffer.
*/

void DistanceAttenuation::process(generator::Generator *gen)
{
    float distance = Location::distanceToOrigin(gen->getLocation());
    float attenuation = (float) (1. / (distance + 1));
    int16_t *tempBuffer = gen->buffer->read();

    for (int i = 0; i < buffer->getDataSize(); i++) {
        tempBuffer[i] = (int16_t) tempBuffer[i] * attenuation;
    }
}

} //end namespace processor

} //end namespace aserver
