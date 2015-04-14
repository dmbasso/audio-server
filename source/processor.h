#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>

#include "generator.h"
#include "sound_buffer.h"

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
 * Abstract base class for all process configuration classes.
 */

class ConfigData {
    // info regarding configuration data for the generator.
};

class AcousticaveConfigData :public ConfigData {

};

/** \brief Parent class for source configuration relative to a specific processor.
 *
 */

struct SourceConfigData {


};

/** \brief Source configuration for the NoOperation processor.
 *
 */

struct NoOperationSourceConfigData :SourceConfigData {

};

/** \brief Source configuration for the DistanceAttenuation processor.
*
*/

struct DistanceAttenuationSourceConfigData :SourceConfigData {
    Location loc;
};

/** \brief Source configuration for the Acouticave processor.
*
*/

struct AcousticaveSourceConfigData :SourceConfigData {

};

/** \brief Container for the generators that will be used by a specific processor.
 *
 */

//\todo Source has a child for every processor
//Source parent implements noOperation processor
//Separate source type and processor type in files

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

class NoOperationSource :public Source {

};

class DistanceAttenuationSource :public Source {

};

/** \brief Contains extra info needed by the Acousticave processor to manage sources.
 *
 */

class AcousticaveSource :public Source {

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

        Processor(unsigned period);
        virtual void config(ConfigData *configData) =0; // the class is abstract
        virtual void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) =0;
        virtual void render()=0;
};

/** \brief A processor that performs no processing on the sources.
 *
 */

class NoOperation :public Processor {
    public:
        NoOperation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void render() override;
};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
 *
 */

class Acousticave :public Processor {
    public:
        Acousticave(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *sourceConfig=nullptr) override;
        void render() override;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d^2 distance law).
 *
 */

class DistanceAttenuation :public Processor {
    public:
        DistanceAttenuation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen, SourceConfigData *srcData =nullptr) override;
        void render() override;
        void process(Source *src);
};

} //end namespace processor
} //end namespace aserver

#endif