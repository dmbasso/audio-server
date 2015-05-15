#ifndef PROCESSOR_DISTANCE_ATTENUATION_H
#define PROCESSOR_DISTANCE_ATTENUATION_H

#include "processor.h"
#include <vector>

namespace aserver {
namespace processor {

enum distanceAttenuationConfigFlags : uint64_t {
    DISTANCE_EXP =              0x1,
    WITH_INPUT_POSITIONS =      0x2,
    DISTANCEATTENUATION_ALL =   0X3
};

struct DistanceAttenuationConfigData : ConfigData {
    ProcessorInput input;
    unsigned short distanceExp = 1;
    bool withInputPositions = false;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d distance law).
*
*/

class DistanceAttenuation :public Processor {
    private:
        unsigned short distanceExp;
        bool withInputPositions;

    public:
        map<unsigned, Location> listenerPositions;

        DistanceAttenuation(unsigned periodSize);
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);
};

} //end namespace processor
} //end namespace aserver

#endif