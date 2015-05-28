#ifndef ACOUSTICAVE_H
#define ACOUSTICAVE_H

#include "processor.h"

extern "C" {
#include <aave.h>
}

namespace aserver {
namespace processor {

enum class aaveHrtf : int16_t {
    MIT = 1,
    CIPIC = 2,
    LISTEN = 3,
    TUB = 4,
    IDENTITY = 5
};

enum acousticaveConfigFlags : uint64_t {
    AAVE_LISTENER_POSITION =          0x1,
    MODEL_FILEPATH =              0x10000,
    GAIN =                        0x20000,
    REFLECTIONS =                 0X40000,
    HRTF =                        0x80000,
    REVERB_ACTIVE =              0x100000,
    RT60 =                       0x200000,
    AREA =                       0x400000,
    VOLUME =                     0x800000,
    AAVE_LISTENER_ORIENTATION = 0x1000000,
    ACOUSTICAVE_ALL =           0x1FF0001
};

/** \brief Listener orientation is set by default facing the positive Y axis.
*/

struct ListenerOrientation {
    float yaw = -M_PI/2.;
    float pitch = 0.;
    float roll = 0.;
};

struct AcousticaveConfigData : ConfigData {
    const char *modelFilePath = "geometries/model.obj";
    float gain = 1;
    uint16_t reflections = 0;
    uint16_t hrtf = 1; // default hrtf set = MIT
    uint16_t reverbActive = 0;
    uint16_t rt60 = 3000;
    uint32_t area = 3000;
    uint32_t volume = 4000;
    ListenerOrientation listenerOrientation;
};

/** \brief Contains data needed by the Acousticave processor to manage sources.
*
*/

class AcousticaveSource :public Source {
    public:
        struct aave_source *aaveSource;
};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
*/

class Acousticave :public Processor {
    private:
        struct aave *aave;
        ListenerOrientation listenerOrientation;

    public:
        Acousticave(uint32_t periodSize);
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void addAaveSource(Source *src);
        void render() override;

        void updateGeometry();
};

} //end namespace processor
} //end namespace aserver

#endif
