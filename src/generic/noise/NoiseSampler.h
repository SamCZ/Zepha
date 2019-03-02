//
// Created by aurailus on 15/02/19.
//

#ifndef ZEUS_NOISESAMPLER_H
#define ZEUS_NOISESAMPLER_H


#include "../helpers/PerlinNoise.h"
#include "NoiseSample.h"
#include "NoiseParams.h"

class NoiseSampler {
public:
    explicit NoiseSampler(unsigned int seed);

    NoiseSample sample(glm::vec3 pos, NoiseParams &params);

private:
    PerlinNoise noise;
    unsigned int seed;
};


#endif //ZEUS_NOISESAMPLER_H
