#ifndef PROCESSOR_DISTANCE_ATTENUATION_H
#define PROCESSOR_DISTANCE_ATTENUATION_H

#include "processor.h"

namespace aserver {
namespace processor {

/** \brief Source configuration for the DistanceAttenuation processor.
*/

struct DistanceAttenuationSourceConfigData :SourceConfigData {
    Location loc;
};

class DistanceAttenuationSource :public Source {

};

/** \brief A processor that processes source distances using a predefined criteria (1/d distance law).
*
*/

class DistanceAttenuation :public Processor {
    public:
        DistanceAttenuation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);
};

} //end namespace processor
} //end namespace aserver

#endif