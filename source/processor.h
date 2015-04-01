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

/** \brief Container for the generators that will be used by a specific processor.
 *
 */

class Source {
    private:
        generator::Generator* gen;
        Location loc;

    public:
        Source() : gen(nullptr) {};
        void setGenerator(aserver::generator::Generator* gen) {this->gen=gen;}
        generator::Generator* getGenerator() {return this->gen;}
        void setLocation(aserver::Location loc) {this->loc = loc;}
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
        std::map<int, Source*> sources;
        unsigned sourceCounter =0;

        Processor(unsigned period);
        virtual void config(ConfigData *configData)=0; // the class is abstract
        virtual void addSource(generator::Generator *gen)=0;
        virtual void render()=0;

        void addBufferData(SoundBuffer *buffer);
};

/** \brief A processor that perfoms no processing on the sources.
 *
 */

class NoOperation :public Processor {
    public:
        NoOperation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
 *
 */

class Acousticave :public Processor {
    public:
        Acousticave(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d^2 distance law).
 *
 */

class DistanceAttenuation :public Processor {
    public:
        DistanceAttenuation(unsigned periodSize) : Processor(periodSize) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
        void process(generator::Generator *gen);
};

} //end namespace processor
} //end namespace aserver

#endif