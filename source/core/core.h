#ifndef CORE_H
#define CORE_H

#include <map>
#include <string>

#include "generator/generator.h"
#include "processor/processor.h"
#include "output/output.h"
#include "sound_buffer.h"

/** \brief  Main namespace for the server, where the top classes reside.
 *
 * Includes several other namespaces for specific program modules.
 */

namespace aserver {

/** \brief Core is the main class for the audio server.
 *
 * It instantiates all other classes and runs the server main loop.
 */

class Core {
    private:
        unsigned periodSize;
        unsigned samplingRate;
        unsigned short generatorCounter;

        std::map<std::string, SoundBuffer*> waves; //string = filename, static
        std::map<int, generator::Generator*> gens;
        processor::Processor *proc;
        output::Output *out;

    public:
        Core() : periodSize(2048), samplingRate(44100), generatorCounter(0), out(nullptr) {};
        unsigned getPeriodSize() {return this->periodSize;}

        int setProcessor(processor::types procType, processor::ConfigData *cfgData = nullptr);
        int addGenerator(generator::types genType, generator::ConfigData *cfgData = nullptr);
        int setOutput(output::types outType);
        void setPeriodSize(unsigned ps) {this->periodSize = ps;}
        void setSamplingRate(unsigned samplingRate) {this->samplingRate = samplingRate;}
        int addSource(processor::SourceConfigData *srcData = nullptr);
        void render();
        void generatorConfig(int gid, generator::ConfigData *configData);  //optional, in case one wishes to alter the generators default values
        void processorConfig(processor::ConfigData *configData);         //optional, in case one wishes to alter the generators default values
        void sourceConfig(int sid, processor::SourceConfigData *srcData);
        void outputConfig(output::OutputConfigData *outputData);
        //void setSourceGenerator(int sid, int gid);
        int readWave(const string filename); //static
        SoundBuffer* getWave(const string filename);
        void shutdown();
};

} //end aserver namespace

#endif
