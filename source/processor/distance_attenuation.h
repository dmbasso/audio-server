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

struct DistanceAttenuationConfigData : ConfigData {
    float motionSamplingRate;
    vector<Location> inputListenerPositions;
};

class DistanceAttenuationSource :public Source {

};

/** \brief A processor that processes source distances using a predefined criteria (1/d distance law).
*
*/

class DistanceAttenuation :public Processor {
    public:
        map<unsigned, Location> listenerPositions;
        vector<Location> inputListenerPositions;
        float motionSamplingRate;
        float periodicPositionRemainder;
    
        DistanceAttenuation(unsigned periodSize) : Processor(periodSize), periodicPositionRemainder(0) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);

        void loadListenerPositions();
        float distanceToListener(Location sourceLocation, unsigned frame);
};

} //end namespace processor
} //end namespace aserver

#endif