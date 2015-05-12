#include "noise.h"

#include <cmath>
#include <iostream>
#include <climits>

using namespace std;

namespace aserver {
namespace generator {

Noise::Noise(unsigned periodSize) : Generator(periodSize)
{
    amplitude = SHRT_MAX/5;
    distType = distributionType::UNIFORM;
}

void Noise::config(const ConfigData *configData)
{
    distType = ((NoiseConfigData *) configData)->distType;
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
