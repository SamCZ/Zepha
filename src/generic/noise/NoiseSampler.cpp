//
// Created by aurailus on 15/02/19.
//

#include "NoiseSampler.h"

NoiseSampler::NoiseSampler(unsigned int seed) {
    this->seed = seed;
}

NoiseSample NoiseSampler::sample(glm::vec3 pos, NoiseParams &params) {
    Simplex::seed(seed);

    if (!params.PERLIN_TYPE) params.SAMPLE_V_PRECISION = 1;
    NoiseSample s(params.SAMPLE_H_PRECISION, params.SAMPLE_V_PRECISION);

    float offsetH = 16.0f / params.SAMPLE_H_PRECISION;
    float offsetV = 16.0f / params.SAMPLE_V_PRECISION;

    for (int i = 0; i <= params.SAMPLE_H_PRECISION; i++) {
        for (int j = 0; j <= params.SAMPLE_V_PRECISION; j++) {
            for (int k = 0; k <= params.SAMPLE_H_PRECISION; k++) {

                float xCoord = (pos.x * 16 + offsetH * i) / params.NOISE_H_FACTOR;
                float yCoord = (params.PERLIN_TYPE) ? (pos.y * 16 + offsetV * j) / params.NOISE_V_FACTOR : 0;
                float zCoord = (pos.z * 16 + offsetH * k) / params.NOISE_H_FACTOR;

                s.set(glm::vec3(i, j, k), (Simplex::noise(glm::vec3(xCoord, yCoord, zCoord)) * 0.5f) * params.NOISE_MULTIPLIER);
            }
        }
    }

    return std::move(s);
}