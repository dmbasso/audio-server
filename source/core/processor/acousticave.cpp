#include "acousticave.h"

namespace aserver {
namespace processor {

Acousticave::Acousticave(unsigned periodSize)  :Processor(periodSize)
{
    // Default listener orientation
    listenerOrientation.yaw = -M_PI/2.;
    listenerOrientation.pitch = 0;
    listenerOrientation.roll = 0;

    aave = (struct aave *) malloc(sizeof *aave);
    aave->gain = 1;
    aave->reverb_active = 0;
    aave->reflections = 0;

    // Default hrtf set
    aave_hrtf_mit(aave);

    // Default geometry
    aave_read_obj(aave, "geometries/model.obj");
}

void Acousticave::config(ConfigData *configData)
{
    auto aaveConfigData = (AcousticaveConfigData*) configData;

    aave_read_obj(aave, aaveConfigData->modelFilePath.c_str());
    aave->reflections = aaveConfigData->reflections;

    aave->gain = aaveConfigData->gain;

    if (aaveConfigData->reverbActive) {
        aave->reverb_active = 1;
        aave_reverb_init(aave);
        aave_reverb_set_area(aave, aaveConfigData->area);
        aave_reverb_set_volume(aave, aaveConfigData->volume);
        aave_reverb_set_rt60(aave, aaveConfigData->rt60);
        aave_reverb_print_parameters(aave, aave->reverb);
    }

    switch ((aaveHrtf) aaveConfigData->hrtf) {
        case aaveHrtf::MIT:
            aave_hrtf_mit(aave);
            break;
        case aaveHrtf::CIPIC:
            aave_hrtf_cipic(aave);
            break;
        case aaveHrtf::LISTEN:
            aave_hrtf_listen(aave);
            break;
        case aaveHrtf::TUB:
            aave_hrtf_tub(aave);
            break;
        case aaveHrtf::IDENTITY:
            aave_hrtf_identity(aave);
            break;
    };
}

void Acousticave::addSource(generator::Generator *gen, SourceConfigData *srcData)
{
    auto source = new processor::AcousticaveSource();
    source->setGenerator(gen);
    source->aaveSource = (struct aave_source *) malloc(sizeof (struct aave_source));
    aave_init_source(aave, source->aaveSource);
    aave_add_source(aave, source->aaveSource);

    if (srcData) {
        source->setLocation(srcData->loc);
    }
    sources[sourceCounter++] = source;
}

void Acousticave::updateGeometry()
{
    // update listener position and orientation
    aave_set_listener_position(aave, listenerPosition.getX(), listenerPosition.getY(), listenerPosition.getZ());
    aave_set_listener_orientation(aave, listenerOrientation.roll,  listenerOrientation.pitch, listenerOrientation.yaw);

    // update sources position
    for (auto const &it : sources) {
        AcousticaveSource *aaveSrc = (AcousticaveSource *) it.second;
        if (!aaveSrc->getGenerator()->locs.empty()) {
            aaveSrc->setLocation(aaveSrc->getGenerator()->locs.rbegin()->second);
            aave_set_source_position(aaveSrc->aaveSource, aaveSrc->getLocation().getX(), aaveSrc->getLocation().getY(), aaveSrc->getLocation().getZ());
        }
    }

    // update aave global geometry
    aave_update(aave);
}

void Acousticave::render()
{
    buffer->reset();
    updateGeometry();

    for (auto const &it : sources) {
        if (it.second->getGenerator()->buffer->getFrameSize() == 2) { //aave input are always mono sources
            for (int i = 0; i < buffer->getPeriodSize(); i++) {
                it.second->getGenerator()->buffer->getData()[i] = ((int16_t) it.second->getGenerator()->buffer->getData()[i * 2] + it.second->getGenerator()->buffer->getData()[i * 2 + 1]) >> 2;
            }
        }
        aave_put_audio(((AcousticaveSource *) it.second)->aaveSource, it.second->getGenerator()->buffer->getData(), buffer->getPeriodSize());
    }
    aave_get_audio(aave, buffer->getData(), buffer->getPeriodSize());
}

} //end namespace processor
} //end namespace aserver