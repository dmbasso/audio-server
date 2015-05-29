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

enum class types : int32_t {
    NO_OPERATION = 1,
    ACOUSTICAVE = 2,
    DISTANCE_ATTENUATION = 3
};

/**
 * Parent class for all process configuration classes.
 */

struct ConfigData {
    uint64_t flags;
    float location[3] = {0., 0., 0.};
};

/** \brief Parent class for source configuration relative to a specific processor.
 *
 */

struct SourceConfigData {
    Location loc;
};

/** This is a temporary class, created for loading the wavePosition file data.
*/

struct ProcessorInput {
    vector<Location> inputListenerPositions;
    float motionSamplingRate;
    float periodicPositionRemainder;
    uint32_t periodSize;

    map<uint32_t, Location> loadListenerPositions();
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
 *  a SoundBuffer \c buffer, where the result of processing each period is stored, and a source scriptPosition.
 *
 *  \c config is used to configure the processor beyond its default values.
 *  \c addSource adds a Source based on the input Generator data.
 *  \c render renders all sources and stores the result on the processors buffer.
 *
 *  Default listener wavePosition = (0,0,0)
 */

class Processor {

    public:
        SoundBuffer *buffer;
        map<int32_t, Source*> sources;
        uint32_t sourceCounter = 0;
        Location listenerPosition;
        ProcessorInput input;

        Processor(uint32_t period);
        virtual void config(ConfigData *configData) =0;
        virtual uint16_t addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) =0;
        virtual void render() =0;
        void setPeriodSize(uint32_t periodSize);
};

} //end namespace processor
} //end namespace aserver

#endif

