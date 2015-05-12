#ifndef NOISE_H
#define NOISE_H

#include "base.h"
#include <random>

using namespace std;

namespace aserver{
namespace generator {

enum class distributionType : int {
    NORMAL = 1,
    UNIFORM = 2
};

struct NoiseConfigData : ConfigData {
    distributionType distType;
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
