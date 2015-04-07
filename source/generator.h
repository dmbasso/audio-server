#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <cstdint>

#include "sound_buffer.h"
#include "location.h"

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
    SIN = 1,
    SQUARE = 2,
    SAWTOOTH = 3
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

class PrimitiveConfigData :public ConfigData {
    private:
        int amplitude = 3277;
        short phase = 0;
        unsigned short frequency = 220;
        unsigned short squareFactor = 10;
        Location location = Location();
        waveformType wft;

    public:
        void setAmplitude(int a) {this->amplitude = a;}
        int getAmplitude() {return this->amplitude;}
        void setPhase(short p) {this->phase = p;}
        short getPhase() {return this->phase;}
        void setFrequency(unsigned short f) {this->frequency = f;}
        unsigned short getFrequency() {return this->frequency;}
        void setSquareFactor(unsigned short sf) {this->squareFactor = sf;}
        unsigned short getSquareFactor() {return this->squareFactor;}
        void setLocation(float x, float y, float z) {this->location = Location(x, y, z);}
        Location getLocation() {return this->location;}
        void setWaveform(waveformType wf) {this->wft=wf;}
        waveformType getWaveformType() {return this->wft;}
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

class Generator {

public:
        Location location; // the start position for the generator.
        //map<int, Location> locations;
        SoundBuffer *buffer;

        Generator(unsigned periodSize);
        virtual ~Generator() {};
        virtual void config(const ConfigData *configdata) =0;
        virtual void render() =0;

        Location getLocation() {return location;}
};

/** \brief Generates a primitive waveform.
 *
 */

class Primitive :public Generator {
    private:
        int phase;
        int frequency;
        int amplitude;
        int squareFactor;
        float lastSampleVal = 1;
        int timeIndex;
        waveformType waveform;

    public:
        Primitive(unsigned periodSize);
        ~Primitive() {};
        void config(const ConfigData *configData) override;
        void render() override;

        int getTimeIndex() {return this->timeIndex;}
};

/**
 * Contains the script instructions for altering the properties of the primitive during playback.
 */

class Test :public Primitive {
    private:
        // time and frequency info
    public:
        virtual void render();

};

class Wave :public Generator {
    private:
//      WavHeader header;
//		int16_t *samples;
//		int length;
//		int channels;

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