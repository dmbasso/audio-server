#ifndef CORE_H
#define CORE_H

#include <map>

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
        uint32_t periodSize;
        uint32_t samplingRate;
        uint16_t generatorCounter;

        std::map<string, SoundBuffer*> waves;
        std::map<int16_t, SoundBuffer*> waveByIndex;
        std::map<string, int16_t> waveIndices;
        int16_t lastWaveIndex = 0;
        std::map<int32_t, generator::Generator*> gens;
        processor::Processor *proc;
        output::Output *out;

    public:
        Core() : periodSize(2048), samplingRate(44100), generatorCounter(0), proc(nullptr), out(nullptr) {};

        int32_t setProcessor(processor::types procType, processor::ConfigData *cfgData = nullptr);
        int16_t addGenerator(generator::types genType, generator::ConfigData *cfgData = nullptr);
        int32_t setOutput(output::types outType, output::ConfigData *cfgData = nullptr);
        uint32_t getPeriodSize() {return periodSize;}
        void setPeriodSize(uint32_t periodSize);
        uint32_t getSamplingRate() {return samplingRate;}
        void setSamplingRate(uint32_t samplingRate) {samplingRate = samplingRate;}
        int16_t addSource(processor::SourceConfigData *srcData = nullptr);
        void render();
        void generatorConfig(int gid, generator::ConfigData *configData);  //optional, in case one wishes to alter the generators default values
        void processorConfig(processor::ConfigData *configData);         //optional, in case one wishes to alter the generators default values
        void sourceConfig(int sid, processor::SourceConfigData *srcData);
        void outputConfig(output::ConfigData *outputData);
        void setSourceGenerator(int16_t sid, int16_t gid);
        SoundBuffer* getWave(string filename);
        SoundBuffer* getWave(int16_t waveIndex);
        int16_t getWaveIndex(string filename);
        int16_t addWave(SoundBuffer *wave);
        void stop_output();
        void reset(bool clearOutput =false);
        uint64_t get_output(int16_t **dest);
        void shutdown();
};

} //end aserver namespace

#endif
