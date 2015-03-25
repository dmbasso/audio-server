#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>

#include "generator.h"

namespace aserver {

/** \brief Contains classes for waveform processing.
 * Currently only one auralization engine is used.
 * It is also possible to use a NoOperation mode to leave the audio unprocessed.
 */

namespace processor {

/**
 * Abstract base class for all process configuration classes.
 */

class ConfigData {
    // info regarding configuration data for the generator.
};

class AcousticaveConfigData : ConfigData {

};

/** \brief Container for the generators that will be used by a specific processor.
 *
 */

class Source {
    private:
        aserver::generator::Generator* generator;
        aserver::Location location;

    public:
        Source() : location(Location()), generator(nullptr) {};
        void setGenerator(aserver::generator::Generator* gen) {this->generator=gen;}
        generator::Generator* getGenerator() {return this->generator;}
        void setLocation(aserver::Location loc) {this->location = loc;}
};

/** \brief Contains extra info needed by the Acousticave processor.
 *
 */

class AcousticaveSource : Source {

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
        unsigned source_counter=0;

        Processor(unsigned period);
        virtual void config(ConfigData *configdata)=0; // the class is abstract
        virtual void addSource(generator::Generator *gen)=0;
        virtual void render()=0;
};

/** \brief A processor that perfoms no processing on the sources.
 *
 */

class NoOperation :public Processor {
    public:
        NoOperation(unsigned period_size) : Processor(period_size) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
};

/** \brief A processor that uses the AcousticAVE auralization engine to process all sources.
 *
 */

class Acousticave :public Processor {
    public:
        Acousticave(unsigned period_size) : Processor(period_size) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
};

/** \brief A processor that processes source distances using a predefined criteria (1/d^2 distance law).
 *
 */

class Distance :public Processor {
    public:
        Distance(unsigned period_size) : Processor(period_size) {};
        void config(ConfigData *configData) override;
        void addSource(generator::Generator *gen) override;
        void render() override;
};

} //end namespace processor
} //end namespace aserver

#endif
