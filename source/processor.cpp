#include <cmath>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
}

void Processor::addBufferData(SoundBuffer *buffer)
{
    for (int i = 0; i < buffer->getDataSize(); i++) {
        this->buffer->data[i] += buffer->data[i];
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
        this->sources.insert(std::pair<int, Source*>(source_counter++,source));
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
    this->sources.insert(std::pair<int, Source*>(source_counter++,source));
}

/** \brief Calculates distance attenuation according to the inverse distance law (used for sound pressure measurements).
*  Calculate distance of each source using its location (the default user position is (0,0,0)
*  Calculate the scalar da = 1/distance
*  Multiply the saclar with every sample in the source rendered buffer.
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

float DistanceAttenuation::distanceBetween2Points(Location x, Location y)
{
    return sqrt( (float) pow(x.x - y.x,2) + pow(x.y - y.y,2) + pow(x.z - y.z,2));
}

float DistanceAttenuation::distanceToOrigin(Location x)
{
    return sqrt( (float) pow(x.x,2) + pow(x.y,2) + pow(x.z,2));
}

void DistanceAttenuation::process(generator::Generator *gen)
{
    float atten = 1. / distanceToOrigin(gen->getLocation());

    for (int i = 0; i < buffer->getDataSize(); i++)
        gen->buffer->data[i] = (int16_t) gen->buffer->data[i] * atten;
}

} //end namespace processor

} //end namespace aserver
