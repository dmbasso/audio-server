#ifndef PROCESSOR_DISTANCE_ATTENUATION_H
#define PROCESSOR_DISTANCE_ATTENUATION_H

#include "processor.h"
#include <vector>

namespace aserver {
namespace processor {

enum distanceAttenuationConfigFlags : uint64_t {
    DISTANCEATTENUATION_LISTENER_POSITION =     0x1,
    DISTANCE_EXP =                          0x10000,
    WITH_INPUT_POSITIONS =                  0x20000,
    DISTANCEATTENUATION_ALL =               0X30001
};

struct DistanceAttenuationConfigData : ConfigData {
    ProcessorInput input;
    uint16_t distanceExp = 1;
    bool withInputPositions = false;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d distance law).
*
*/

class DistanceAttenuation :public Processor {
    private:
        uint16_t distanceExp;
        bool withInputPositions;

    public:
        map<uint32_t, Location> listenerPositions;

        DistanceAttenuation(uint32_t periodSize);
        void config(ConfigData *configData) override;
        uint16_t addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);
};

} //end namespace processor
} //end namespace aserver

#endif