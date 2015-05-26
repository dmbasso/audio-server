#ifndef NOISE_H
#define NOISE_H

#include "base.h"
#include <random>
#include <climits>

using namespace std;

namespace aserver{
namespace generator {

enum class distributionType : int32_t {
    NORMAL = 1,
    UNIFORM = 2
};

enum noiseConfigFlags : uint64_t {
    NOISE_AMPLITUDE =   0x1,
    DISTRIBUTION =      0x2,
    NOISE_ALL =         0X3
};

#pragma pack(1)  // force byte-alignment

struct NoiseConfigData : ConfigData {
    uint32_t amplitude = SHRT_MAX;
    distributionType distType = distributionType::UNIFORM;
};

#pragma pack()


class Noise :public Generator {
    protected:
        uint32_t amplitude;
        distributionType distType;
        default_random_engine generator;

    public:
        Noise(Core *core, uint32_t periodSize);
        ~Noise() {};
        void config(const ConfigData *configData) override;
        void render() override;
};

} //end generator namespace
} //end aserver namespace

#endif
