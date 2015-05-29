#ifndef PROCESSOR_NOOPERATION_H
#define PROCESSOR_NOOPERATION_H

#include "processor.h"

namespace aserver {
namespace processor {

/** \brief A processor that performs no processing on the sources.
*
*/

class NoOperation :public Processor {
    public:
        NoOperation(uint32_t periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) {};
        uint16_t addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void render() override;
};

} //end namespace processor
} //end namespace aserver

#endif