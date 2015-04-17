#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>

#include "../generator/generator.h"
#include "../sound_buffer.h"
#include "../location.h"

namespace aserver {

/** \brief Contains classes for waveform processing.
 * Currently only one auralization engine is used.
 * It is also possible to use a NoOperation mode to leave the audio unprocessed.
 */

namespace processor {

enum class types : int {
    NO_OPERATION = 1,
    ACOUSTICAVE = 2,
    DISTANCE_ATTENUATION = 3
};

/**
 * Parent class for all process configuration classes.
 */

class ConfigData {
    // info regarding configuration data for the generator.
};

/** \brief Parent class for source configuration relative to a specific processor.
 *
 */

struct SourceConfigData {

};

/** \brief Container for the generators that will be used by a specific processor.
 *
 */

//\todo Source parent implements noOperationSource processor

class Source {
    private:
        generator::Generator* gen;
        Location loc;

    public:
        Source() : gen(nullptr) {}
        void setGenerator(generator::Generator* gen) {this->gen = gen;}
        generator::Generator* getGenerator() {return this->gen;}
        void setLocation(Location _loc) {this->loc = _loc;}
        Location getLocation() {return this->loc;}
};

/** \brief Abstract base class for the audio processors.
 *  This class declares the virtual methods required for basic processor functionality.
 *  It contains a \c map of Source, that are processed by calling the \c render method,
 *  a SoundBuffer \c buffer, where the result of processing each period is stored, and a source counter.
 *
 *  \c config is used to configure the processor beyond its default values.
 *  \c addSource adds a Source based on the input Generator data.
 *  \c render renders all sources and stores the result on the processors buffer.
 */

class Processor {

    public:
        SoundBuffer *buffer;
        map<int, Source*> sources;
        unsigned sourceCounter =0;
        Location listenerPosition;

        Processor(unsigned period);
        virtual void config(ConfigData *configData) =0; // the class is abstract
        virtual void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) =0;
        virtual void render() =0;
};

} //end namespace processor
} //end namespace aserver

#endif