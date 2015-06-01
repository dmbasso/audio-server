#ifndef GENERATOR_BASE_H
#define GENERATOR_BASE_H

#include <map>
#include <cstdint>
#include <math.h>

#include "../sound_buffer.h"
#include "../location.h"

using namespace std;

namespace aserver {

class Core;

/** \brief Contains classes for waveform generation.
 */

namespace generator {

enum class types : int32_t {
    PRIMITIVE = 1,
    WAVE = 2,
    TEST = 3,
    SCRIPT = 4,
    NOISE = 5
};

enum class waveformType : int32_t {
    SINE = 1,
    SQUARE = 2,
    SAWTOOTH = 3
};

enum class playbackCommand : int32_t {
    PLAY = 1,
    STOP = 2,
    PAUSE = 3,
    PLAY_LOOP = 4,
    REVERSE = 5
};

enum class playbackState : int16_t {
    PLAYING = 1,
    PLAYING_LOOP = 2,
    STOPPED = 3,
    PAUSED = 4,
    REWINDING = 5
};

/** \brief Base class for all data configuration classes.
 */

#pragma pack(1)  // force byte-alignment

struct ConfigData {
    uint64_t flags;
};

#pragma pack()

/** \brief Generator is the abstract base class for all sound generators.
 *  Its concrete types are SoundBuffer and a Location.
 *  Contains two virtual methods, one to set things up and one to
 *  render the next period (this base class only renders silence).
 */

// Locations is a map<uint32_t bufferIndex, Location loc> that stores location changes in each buffer.
// This map will be used to update the location of each source when rendered (source.render)

class Generator {
    protected:
        Core *core;

    public:
        map<uint32_t, Location> locs;
        SoundBuffer *buffer;

        Generator(Core *core, uint32_t periodSize);
        virtual ~Generator() {};
        virtual void config(const ConfigData *configdata) =0;
        virtual void render() =0;
        void setPeriodSize(uint32_t periodSize);
};

} //end generator namespace
} //end aserver namespace

#endif

