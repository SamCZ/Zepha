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
    struct MapGenJob {
        std::vector<int>
            elevation,
            biome,
            *blocks;
        glm::vec3 pos;

        explicit MapGenJob(glm::vec3 pos) {
            this->pos = pos;
            elevation.reserve(4096);
            biome.reserve(4096);
            blocks = new std::vector<int>(4096);
        }
    };

    void getElevation(MapGenJob& j);
    void getBiome(MapGenJob& j);
    void fillBlocks(MapGenJob& j);

    void init(unsigned int seed);

    PerlinNoise p;
};


#endif //ZEUS_MAPGEN_H
