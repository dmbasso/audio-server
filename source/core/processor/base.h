#ifndef PROCESSOR_BASE_H
#define PROCESSOR_BASE_H

#include <map>
#include <vector>

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

struct ConfigData {
    uint64_t flags;
};

/** \brief Parent class for source configuration relative to a specific processor.
 *
 */

struct SourceConfigData {
    Location loc;
};

/** This is a temporary class, created for loading the position file data.
*/

struct ProcessorInput {
    vector<Location> inputListenerPositions;
    float motionSamplingRate;
    float periodicPositionRemainder;
    unsigned periodSize;

    map<unsigned, Location> loadListenerPositions();
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
        virtual void config(SourceConfigData *srcData);
        void setGenerator(generator::Generator* gen) {this->gen = gen;}
        generator::Generator* getGenerator() {return this->gen;}
        void setLocation(Location loc) {this->loc = loc;}
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
 *
 *  Default listener position = (0,0,0)
 */

class Processor {

    public:
        SoundBuffer *buffer;
        map<int, Source*> sources;
        unsigned sourceCounter = 0;
        Location listenerPosition;
        ProcessorInput input;

        Processor(unsigned period);
        virtual void config(ConfigData *configData) =0; // the class is abstract
        virtual void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) =0;
        virtual void render() =0;
};

} //end namespace processor
} //end namespace aserver

#endif

