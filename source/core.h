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
    ACOUSTICAVE = 2
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
        unsigned period_size;    /// The number of frames to be processed in each iteration.
        std::map<std::string, SoundBuffer*> waves;
        std::map<int, generator::Generator*> generators;
        processor::Processor *processor;
        output::Output *output;

        unsigned short generator_counter;

    public:
        Core() : period_size(2048), generator_counter(0) {};
        unsigned getPeriodSize() {return this->period_size;}

        int setProcessor(processorType procType);
        int addGenerator(generatorType genType);
        int setOutput(outputType outType);
        void setPeriodSize(unsigned ps) {this->period_size = ps;}
        //void generatorConfig(int id, generator::ConfigData *configData);  //optional, in case one wishes to alter the generators default values
        //void processorConfig(processor::ConfigData *configData)           //optional, in case one wishes to alter the generators default values
        int addSource(); //get the last generator added and place it in the processor source map.
        //void setSourceGenerator(int sid, int gid)
        void render(unsigned write_periods);  //this method must change to acomodate other outputs...
};

} //end aserver namespace

#endif
