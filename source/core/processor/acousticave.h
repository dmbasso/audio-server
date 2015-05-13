#ifndef ACOUSTICAVE_H
#define ACOUSTICAVE_H

#include "processor.h"

extern "C" {
#include <aave.h>
}

namespace aserver {
namespace processor {

enum class aaveHrtf : unsigned short {
    MIT = 1,
    CIPIC = 2,
    LISTEN = 3,
    TUB = 4,
    IDENTITY = 5
};

enum acousticaveConfigFlags : uint64_t {
    MODEL_FILEPATH =    0x1,
    GAIN =              0x2,
    REFLECTIONS =       0X4,
    HRTF =              0x8,
    REVERB_ACTIVE =    0x10,
    RT60 =             0x20,
    AREA =             0x40,
    VOLUME =           0x80,
    ACOUSTICAVE_ALL =  0xFF
};

struct AcousticaveConfigData : ConfigData {
    const char *modelFilePath = "geometries/model.obj";
    float gain = 1;
    unsigned short reflections = 0;
    unsigned short hrtf = 1; // default hrtf set = MIT
    unsigned short reverbActive = 0;
    unsigned short rt60 = 3000;
    unsigned area = 3000;
    unsigned volume = 4000;
};

/** \brief Contains extra info needed by the Acousticave processor to manage sources.
*
*/

class AcousticaveSource :public Source {
    public:
        struct aave_source *aaveSource;
};

struct ListenerOrientation {
    float yaw;
    float pitch;
    float roll;
};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
*
*/

class Acousticave :public Processor {
    private:
        struct aave *aave;
        ListenerOrientation listenerOrientation;

    public:
        Acousticave(unsigned periodSize);
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void render() override;

        void updateGeometry();
};

} //end namespace processor
} //end namespace aserver

#endif
