#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <cstdint>

#include "sound_buffer.h"
#include "location.h"

using namespace std;

namespace aserver {

/** \brief Contains classes for waveform generation.
 */

namespace generator {

enum class types : int {
    PRIMITIVE = 1,
    WAVE = 2,
    TEST = 3,
    SCRIPT = 4
};

enum class waveformType : int {
    SINE = 1,
    SQUARE = 2,
    SAWTOOTH = 3
};

enum  class playbackCommand : int {
    PLAY = 1,
    STOP = 2,
    PAUSE = 3,
    PLAY_LOOP = 4,
    REVERSE = 5
};

enum  class playbackState : int {
    PLAYING = 1,
    STOPPED = 2,
    PAUSED = 3,
    REWINDING = 4
};

/** \brief Base class for all data configuration classes.
 */

class ConfigData {
    // info regarding general configuration data for a generator.

};

/** \brief PrimitiveConfigData sets the configuration parameters for a Primitive generator.
 *  These parameters are set with their default values within the Primitive generator constructor.
 *  The purpose of this class is to be used if there is need to alter the default parameters.
 */

    //\todo remove get/set, replace class with struct

class PrimitiveConfigData :public ConfigData {
    public:
        int amplitude = 3277;
        unsigned short frequency = 220;
        unsigned short squareFactor = 10;
        waveformType wft = waveformType::SINE;
        Location location = Location();
};

/** \brief Keyframe and other information for scripting sound files.
 */

class ScriptConfigData :public ConfigData {

};

/** \brief Test configuration for the signal primitives generated.
 */

class TestConfigData :public ConfigData {

};

/** \brief Generator is the abstract base class for all sound generators.
 *  Its concrete types are SoundBuffer and a Location.
 *  Contains two virtual methods, one to set things up and one to
 *  render the next period (this base class only renders silence).
 */

// Locations is a map<unsigned bufferIndex, Location loc> that stores location changes in each buffer.
// This map will be used to update the location of each source when rendered (source.render)
// \todo For that we must create a map iterator to check the next map pair for location changes.
// \todo Only source should have a location; Source config should be implemented.

class Generator {

public:
        map<unsigned, Location> locs;
        SoundBuffer *buffer;

        Generator(unsigned periodSize);
        virtual ~Generator() {};
        virtual void config(const ConfigData *configdata) =0;
        virtual void render() =0;
};

/** \brief Generates a primitive waveform.
 *
 */

class Primitive :public Generator {
    protected:
        float phase;
        float frequency;
        int amplitude;
        int squareFactor;
        waveformType waveform;

    public:
        Primitive(unsigned periodSize);
        ~Primitive() {};
        void config(const ConfigData *configData) override;
        void render() override;
        void renderNFrames(unsigned start, unsigned end);
};

/**
 * Contains the script instructions for altering the properties of the primitive during playback.
 * This script is a map with key=frame  and Entry=Location
 */

class Test :public Primitive {
    private:
        unsigned transitionPeriod;
        unsigned remainingFrames;
        float frequencyScaleFactor;
        float locationScaleFactor;

    public:
        Test(unsigned periodSize);
        ~Test() {};
        void config(const ConfigData *configData) override;
        virtual void render() override;
};

class Wave :public Generator {
    private:
//		SoundBuffer *soundFile;
        //playbackState playbackState = playbackState::STOPPED;
        // float position=0;
        // float increment; //step size to read buffer -> determines new frequency output

    public:
        Wave(unsigned periodSize);
        virtual ~Wave() override;
        virtual void config(const ConfigData *configData) override;
        virtual void render() override;
};

class Script :public Wave {
    private:
        //keyframe and other info
    public:
        virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif