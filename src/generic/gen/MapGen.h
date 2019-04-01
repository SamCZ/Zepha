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
    void getDensityMap(MapGenJob &job);
    void getElevation(MapGenJob &j);

    void fillChunk(MapGenJob &j);

    NoiseSampler sampler;

    NoiseParams p_feature;
    NoiseParams p_feature_scale;
    NoiseParams p_density;
    NoiseParams p_density_variation;
    NoiseParams p_density_variation_smaller;

    NoiseParams p_flora_feature;
    NoiseParams p_flora_smaller;
};


#endif //ZEUS_MAPGEN_H