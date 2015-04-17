#ifndef CORE_H
#define CORE_H

#include <map>
#include <string>

#include "generator/generator.h"
#include "processor/processor.h"
#include "output.h"
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
        Core() : periodSize(2048), samplingRate(44100), generatorCounter(0) {};
        unsigned getPeriodSize() {return this->periodSize;}

        int setProcessor(processor::types procType);
        int addGenerator(generator::types genType, generator::ConfigData *cfgdata = nullptr);
        int setOutput(output::types outType);
        void setPeriodSize(unsigned ps) {this->periodSize = ps;}
        void setSamplingRate(unsigned samplingRate) {this->samplingRate = samplingRate;}
        int addSource(processor::SourceConfigData *srcData = nullptr);
        void render(unsigned writePeriods);
        //void generatorConfig(int id, generator::ConfigData *configData);  //optional, in case one wishes to alter the generators default values
        //void processorConfig(processor::ConfigData *configData)           //optional, in case one wishes to alter the generators default values
        //void setSourceGenerator(int sid, int gid)
        int readWave(const string filename); //static
        SoundBuffer* getWave(const string filename);

    //\todo readListenerPosition(); 12 (nframes) -> 12.3 123.2 123.3 (one frame)
    //\store x,y,z in memory  - framerate for position update 240Hz = 4.16 msecs
};

    //\todo processorConfig: loads at each frame the procConfigData (will update processor listenerPosition)
} //end aserver namespace

#endif
