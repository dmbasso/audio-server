#ifndef GENERATOR_H
#define GENERATOR_H

#include <map>
#include <cstdint>

namespace aserver {

/** \brief SoundBuffer is a container for blocks of size \c period_size of audio frames with size \c frame_size.
 *
 *  \c period_size corresponds to the size of each period of audio frames that are processed.
 *  It is set by the Core class in the server initialization.
 *  \c frame_size is the number of 16 bit audio samples per audio frame (mono = 1, stereo = 2 (default));
 *  \c data is a pointer to an array of \c int16_t type variables. It contains the 16 bit audio samples correnponding to one period. The SoundBuffer constructor initialization allocates this pointer, an \c int16_t vector of size = \c frame_size * \c period_size, and zeros all values;
 *
 *  \todo Make data private and use get/set.
 */

class SoundBuffer {

    private:
        unsigned short frame_size;
        unsigned period_size;

    public:
        int16_t *data;

        SoundBuffer(unsigned period_size);
        unsigned getDataSize() {return frame_size * period_size;}
        unsigned getPeriodSize () {return period_size;}
        void addData(int16_t *data);
        int16_t* getData() {return data;}
        void reset();
};

/** \brief Location contains a 3D space location.
 *
 */

class Location {
    private:
        float x, y, z;
    public:
        Location() : x(0.), y(0.), z(0.) {}
        Location(float x, float y, float z) : x(x), y(y), z(z) {}
};

/** \brief Contains classes for waveform generation.
 */

namespace generator {

/** \brief Base class for all data configuration classes.
 */

class ConfigData {
    // info regarding general configuration data for a generator.

};

/** \brief PrimitiveConfigData sets the configuration paramenters for a Primitive generator.
 *  These parameters are set with their default values within the Primitive generator constructor.
 *  The purpose of this class is to be used if there is need to alter the default parameters.
 */

class PrimitiveConfigData :public ConfigData {
    public:
        int amplitude;
        int phase;
        int frequency;
        int squareFactor;

        PrimitiveConfigData();
};

/** \brief Keyframe and other information for scripting sound files.
 */

class ScriptConfigData : ConfigData {

};

/** \brief Test configuration for the signal primitives generated.
 */

class TestConfigData : ConfigData {

};

/** \brief Generator is the abstract base class for all sound generators.
 *  Its concrete types are SoundBuffer and a Location.
 *  Contains two virtual methods, one to set things up and one to
 *  render the next period (this base class only renders silence).
 */

class Generator {

    private:
        Location location; // the start position for the generator.

    public:
        SoundBuffer *buffer;

        Generator(unsigned period_size);
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
        int time_index;

    public:
        Primitive(unsigned period_size);
        virtual ~Primitive() override;
        virtual void config(const ConfigData *configdata) override;
        virtual void render() override;

        int getTimeIndex() {return this->time_index;}
};

/**
 * Contains the script instructions for altering the properties of the primitive during playback.
 */

class Test : Primitive {
    private:
        // time and frequency info
    public:
        virtual void render();

};

class Wave : Generator {
    private:
//      WavHeader header;
//		int16_t *samples;
//		int length;
//		int channels;

    public:
        Wave(unsigned period_size);
        virtual ~Wave() override;
        virtual void config(const ConfigData *configdata) override;
        virtual void render() override;
};

class Script : Wave {
    private:
        //keyframe and other info
    public:
        virtual void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
