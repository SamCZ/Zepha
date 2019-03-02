//
// Created by aurailus on 28/01/19.
//

#ifndef ZEUS_MAPGEN_H
#define ZEUS_MAPGEN_H


#include <vec3.hpp>
#include <vector>

#include "MapGenJob.h"
#include "../blocks/BlockChunk.h"
#include "../noise/NoiseParams.h"
#include "../noise/NoiseSampler.h"

class MapGen {
public:
    explicit MapGen(unsigned int seed);
    BlockChunk* generate(glm::vec3 pos);
private:
    void buildElevation(MapGenJob &j);
    void fillChunk(MapGenJob &j);

    NoiseSampler sampler;

    NoiseParams p_feature;
    NoiseParams p_feature_scale;
    NoiseParams p_elevation;
    NoiseParams p_elevation_variation;
    NoiseParams p_elevation_variation_smaller;
};


#endif //ZEUS_MAPGEN_H