//
// Created by aurailus on 28/01/19.
//

#ifndef ZEUS_MAPGEN_H
#define ZEUS_MAPGEN_H


#include <vec3.hpp>
#include <vector>

#include "MapGenJob.h"
#include "../blocks/BlockChunk.h"

#include <noise/noise.h>

using namespace noise;

class MapGen {
public:
    explicit MapGen(unsigned int seed);
    BlockChunk* generate(glm::vec3 pos);
private:
    void getDensityMap(MapGenJob &job);
    void getElevation(MapGenJob &j);

    void fillChunk(MapGenJob &j);

    unsigned int seed;

    module::RidgedMulti terrainMountains;
    module::Billow terrainFlatBase;
    module::ScaleBias terrainFlat;
    module::Perlin terrainType;
    module::Select terrainFinal;

    module::Perlin grassNoise;
    module::Turbulence grassTurbulence;
    module::ScaleBias grassFinal;

    module::Perlin floraNoise;
    module::Turbulence floraTurbulence;
    module::ScaleBias floraFinal;

    module::Perlin floraDensity;

    module::Perlin biomeTemp;
};


#endif //ZEUS_MAPGEN_H