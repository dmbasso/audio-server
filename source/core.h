#ifndef CORE_H
#define CORE_H

#include <map>
#include <string>

#include "generator.h"
#include "processor.h"
#include "output.h"

/** \brief  Main namespace for the server, where the top classes reside.
 *
 * Includes several other namespaces for specific program modules.
 */

namespace aserver {

enum class generatorType : int {
    PRIMITIVE = 1,
    WAVE = 2,
    TEST = 3,
    SCRIPT = 4
};

enum class processorType : int {
    NOOPERATION = 1,
    ACOUSTICAVE = 2,
    DISTANCEATTENUATION = 3
};

enum class outputType : int {
    FILE = 1,
    ALSA = 2,
    MEMORY = 3
};

/** \brief Core is the main class for the audio server.
 *
 * It instantiates all other classes and runs the server main loop.
 */

class Core {
    private:
        unsigned period_size;
        unsigned sampling_rate;
        unsigned short generator_counter;

        std::map<std::string, SoundBuffer*> waves;
        std::map<int, generator::Generator*> generators;
        processor::Processor *processor;
        output::Output *output;

    public:
        Core() : period_size(2048), sampling_rate(44100), generator_counter(0) {};
        unsigned getPeriodSize() {return this->period_size;}

        int setProcessor(processorType procType);
        int addGenerator(generatorType genType, generator::ConfigData *cfgdata);
        int setOutput(outputType outType);
        void setPeriodSize(unsigned ps) {this->period_size = ps;}
        void setSamplingRate(unsigned sampling_rate) {this->sampling_rate = sampling_rate;}
        int addSource();
        void renderFile(unsigned write_periods);
        //void generatorConfig(int id, generator::ConfigData *configData);  //optional, in case one wishes to alter the generators default values
        //void processorConfig(processor::ConfigData *configData)           //optional, in case one wishes to alter the generators default values
        //void setSourceGenerator(int sid, int gid)
};

} //end aserver namespace

#endif
