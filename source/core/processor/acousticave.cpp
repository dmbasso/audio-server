#include <fstream>
#include <iostream>
#include "acousticave.h"

namespace aserver {
namespace processor {

Acousticave::Acousticave(uint32_t periodSize)  :Processor(periodSize)
{
    aave = (struct aave *) malloc(sizeof *aave);

    AcousticaveConfigData *cfgData = new AcousticaveConfigData();
    cfgData->flags = acousticaveConfigFlags::ACOUSTICAVE_ALL;
    config(cfgData);
}

void Acousticave::config(ConfigData *configData)
{
    AcousticaveConfigData *cfgData = (AcousticaveConfigData*) configData;

    if (cfgData->flags & acousticaveConfigFlags::AAVE_LISTENER_POSITION) {
        listenerPosition = Location(cfgData->location[0], cfgData->location[1], cfgData->location[2]);
    }
    if (cfgData->flags & acousticaveConfigFlags::AAVE_LISTENER_ORIENTATION) {
        listenerOrientation.yaw = cfgData->listenerOrientation.yaw;
        listenerOrientation.pitch = cfgData->listenerOrientation.pitch;
        listenerOrientation.roll = cfgData->listenerOrientation.roll;
    }
    if (cfgData->flags & acousticaveConfigFlags::MODEL_FILEPATH) {
        ifstream ifs(cfgData->modelFilePath);
        if (ifs.good()) {
            aave_read_obj(aave, cfgData->modelFilePath);
        }
        else {
            cout << "Error opening model geometry file = " << cfgData->modelFilePath << endl;
            return;
        }
    }
    if (cfgData->flags & acousticaveConfigFlags::GAIN) {
        aave->gain = cfgData->gain;
    }
    if (cfgData->flags & acousticaveConfigFlags::REFLECTIONS) {
        aave->reflections = cfgData->reflections;
    }
    if (cfgData->flags & acousticaveConfigFlags::HRTF) {
        switch ((aaveHrtf) cfgData->hrtf) {
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
    if (cfgData->flags & acousticaveConfigFlags::REVERB_ACTIVE) {
        aave->reverb_active = cfgData->reverbActive;
        if (aave->reverb_active) {
            aave_reverb_init(aave);
        }
    }
    if (cfgData->flags & acousticaveConfigFlags::RT60) {
        if (aave->reverb_active) {
            aave_reverb_set_rt60(aave, cfgData->rt60);
        }
    }
    if (cfgData->flags & acousticaveConfigFlags::AREA) {
        if (aave->reverb_active) {
            aave_reverb_set_area(aave, cfgData->area);
        }
    }
    if (cfgData->flags & acousticaveConfigFlags::VOLUME) {
        if (aave->reverb_active) {
            aave_reverb_set_volume(aave, cfgData->volume);
        }
    }
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
            for (int32_t i = 0; i < buffer->getPeriodSize(); i++) {
                it.second->getGenerator()->buffer->getData()[i] = ((int16_t) it.second->getGenerator()->buffer->getData()[i * 2] + it.second->getGenerator()->buffer->getData()[i * 2 + 1]) >> 2;
            }
        }
        aave_put_audio(((AcousticaveSource *) it.second)->aaveSource, it.second->getGenerator()->buffer->getData(), buffer->getPeriodSize());
    }
    aave_get_audio(aave, buffer->getData(), buffer->getPeriodSize());
}

} //end namespace processor
} //end namespace aserver