//
// Created by aurailus on 28/01/19.
//

#ifndef ZEUS_MAPGEN_H
#define ZEUS_MAPGEN_H


#include <vec3.hpp>
#include <vector>
#include "../blocks/BlockChunk.h"
#include "../helpers/PerlinNoise.h"

class MapGen {
public:
    MapGen() : p(1) { init(1); }
    explicit MapGen(unsigned int seed) : p(seed) { init(seed); }

    BlockChunk* generate(glm::vec3 pos);

private:
    //Base 3D Perlin Noise
    const float BASE_H_FACTOR = 22.0f;
    const float BASE_V_FACTOR = 44.0f;
    const int BASE_H_PRECISION = 2;
    const int BASE_V_PRECISION = 4;

    //Hillyness Factors
    const float HILLS_H_FACTOR = 250.0f;
    const float HILLS_BASE_EFFECT = 120.0f;
    const float HILLS_EFFECT_MULT = 0.2f;

    typedef std::vector<std::vector<std::vector<float>>> noise_points;

    float noiseAt(float x, float y, float z);
    noise_points getNoisePoints(glm::vec3 pos, int hScale, int vScale);

    struct MapGenJob {
        std::vector<int>
            elevation,
            biome,
            *blocks;
        glm::vec3 pos = glm::vec3(0, 0, 0);

        explicit MapGenJob(glm::vec3 pos) {
            this->pos = pos;
            elevation.reserve(4096);
            biome.reserve(4096);
            blocks = new std::vector<int>(4096);
        }
    };

    void init(unsigned int seed);

    PerlinNoise p;
};


#endif //ZEUS_MAPGEN_H
