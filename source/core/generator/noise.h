#ifndef NOISE_H
#define NOISE_H

#include "base.h"
#include <random>
#include <climits>

using namespace std;

namespace aserver{
namespace generator {

enum class distributionType : int {
    NORMAL = 1,
    UNIFORM = 2
};

enum noiseConfigFlags : uint64_t {
    NOISE_AMPLITUDE =   0x1,
    DISTRIBUTION =      0x2,
    NOISE_ALL =         0X3
};

struct NoiseConfigData : ConfigData {
    unsigned amplitude = SHRT_MAX * 0.2;
    distributionType distType = distributionType::UNIFORM;
};

class Noise :public Generator {
    protected:
        unsigned amplitude;
        distributionType distType;
        default_random_engine generator;

    public:
        Noise(unsigned periodSize);
        ~Noise() {};
        void config(const ConfigData *configData) override;
        void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
