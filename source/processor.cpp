#include <iostream>

#include "processor.h"

using namespace std;

namespace aserver {
namespace processor {

Processor::Processor(unsigned period)
{
    buffer = new SoundBuffer(period);
}

void NoOperation::config(ConfigData *configData)
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

void NoOperation::addSource(generator::Generator *gen)
{
        auto source = new processor::Source();
        source->setGenerator(gen);
        source->setLocation(gen->getLocation());
        this->sources.insert(std::pair<int, Source*>(source_counter++,source));
}

void NoOperation::render()
{
    cout << "sources size = " << sources.size() << endl;
    //Should we iterate the generators in the processors source map or the core generators?
    //for (map<int, Source*>::iterator it=sources.begin(); it!=sources.end(); ++it) {

    this->buffer->reset();

    for (auto const &it : sources) {
        cout << "iterate one source\n";
        auto gen = it.second->getGenerator();
        gen->render();
    }

    cout << "Finished rendering all sources...\n";

    for (auto const &it : sources) {
        auto gen = it.second->getGenerator();
        this->buffer->addData(gen->buffer->getData());
    }

    cout << "Finished mixing all sources...\n";
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

} //end namespace processor

} //end namespace aserver
