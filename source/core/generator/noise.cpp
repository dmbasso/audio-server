#include "noise.h"

using namespace std;

namespace aserver {
namespace generator {

Noise::Noise(Core *core, unsigned periodSize) : Generator(core, periodSize)
{
    NoiseConfigData *cfgData = new NoiseConfigData();
    cfgData->flags = noiseConfigFlags::NOISE_ALL;
    config(cfgData);
}

void Noise::config(const ConfigData *configData)
{
    NoiseConfigData *cfgData = (NoiseConfigData *) configData;

    if (cfgData->flags & noiseConfigFlags::NOISE_AMPLITUDE) {
        amplitude = cfgData->amplitude;
    }

    if (cfgData->flags & noiseConfigFlags::DISTRIBUTION) {
        distType = cfgData->distType;
    }
}

void Noise::render()
{
    int16_t sams[2];    
    float val;
    
    switch (distType) {
        case distributionType::UNIFORM: {
            uniform_int_distribution<int> uniform(-amplitude, amplitude); 
            for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {                
                val = uniform(generator);
                sams[0] = sams[1] = (int16_t) val;
                buffer->writeFrame(sams, i);
            }
            break;
            }
        case distributionType::NORMAL: {
            normal_distribution<double> normal(0, amplitude);
            for (unsigned i = 0; i < buffer->getPeriodSize(); i++) {
                val = normal(generator);
                sams[0] = sams[1] = (int16_t) val;
                buffer->writeFrame(sams, i);
            }
            break;
            }
    }
}

} //end generator namespace
} //end aserver namespace
