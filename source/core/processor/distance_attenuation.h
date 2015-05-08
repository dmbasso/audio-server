#ifndef PROCESSOR_DISTANCE_ATTENUATION_H
#define PROCESSOR_DISTANCE_ATTENUATION_H

#include "processor.h"
#include <vector>

namespace aserver {
namespace processor {

struct DistanceAttenuationConfigData : ConfigData {
    ProcessorInput input;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d distance law).
*
*/

class DistanceAttenuation :public Processor {
    public:
        map<unsigned, Location> listenerPositions;

        DistanceAttenuation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);
};

} //end namespace processor
} //end namespace aserver

#endif