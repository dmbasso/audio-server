#ifndef GENERATOR_BASE_H
#define GENERATOR_BASE_H

#include <map>
#include <cstdint>
#include <math.h>

#include "../sound_buffer.h"
#include "../location.h"

using namespace std;

namespace aserver {

/** \brief Contains classes for waveform generation.
 */

namespace generator {

enum class types : int {
    PRIMITIVE = 1,
    WAVE = 2,
    TEST = 3,
    SCRIPT = 4,
    NOISE = 5
};

enum class waveformType : int {
    SINE = 1,
    SQUARE = 2,
    SAWTOOTH = 3
};

enum class playbackCommand : int {
    PLAY = 1,
    STOP = 2,
    PAUSE = 3,
    PLAY_LOOP = 4,
    REVERSE = 5
};

enum class playbackState : int {
    PLAYING = 1,
    STOPPED = 2,
    PAUSED = 3,
    REWINDING = 4
};

/** \brief Base class for all data configuration classes.
 */

struct ConfigData {
    uint64_t flags;
};

/** \brief Generator is the abstract base class for all sound generators.
 *  Its concrete types are SoundBuffer and a Location.
 *  Contains two virtual methods, one to set things up and one to
 *  render the next period (this base class only renders silence).
 */

// Locations is a map<unsigned bufferIndex, Location loc> that stores location changes in each buffer.
// This map will be used to update the location of each source when rendered (source.render)

class Generator {

public:
        map<unsigned, Location> locs;
        SoundBuffer *buffer;

        Generator(unsigned periodSize);
        virtual ~Generator() {};
        virtual void config(const ConfigData *configdata) =0;
        virtual void render() =0;
};

} //end generator namespace
} //end aserver namespace

#endif

