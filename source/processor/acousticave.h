#ifndef PROCESSOR_ACOUSTICAVE_H
#define PROCESSOR_ACOUSTICAVE_H

#include "processor.h"

namespace aserver {
namespace processor {

class AcousticaveConfigData :public ConfigData {

};

/** \brief Source configuration for the Acouticave processor.
*
*/

struct AcousticaveSourceConfigData :SourceConfigData {

};

/** \brief Contains extra info needed by the Acousticave processor to manage sources.
*
*/

class AcousticaveSource :public Source {

};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
*
*/

class Acousticave :public Processor {
public:
    Acousticave(unsigned periodSize) : Processor(periodSize) {};
    void config(ConfigData *configData) override;
    void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
    void render() override;
};

} //end namespace processor
} //end namespace aserver

#endif
