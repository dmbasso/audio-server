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
        NoOperation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void render() override;
};

} //end namespace processor
} //end namespace aserver

#endif