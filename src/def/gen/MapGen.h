//
// Created by aurailus on 28/01/19.
//

#ifndef ZEUS_MAPGEN_H
#define ZEUS_MAPGEN_H


#include <vec3.hpp>
#include <vector>

#include "MapGenJob.h"
#include "../LocalDefs.h"
#include "../../world/chunk/BlockChunk.h"

#include <noise/noise.h>

using namespace noise;

class MapGen {
public:
    MapGen(unsigned int seed, BlockAtlas& atlas);
    BlockChunk* generate(glm::vec3 pos);
private:

    void getDensityMap(MapGenJob &job);
    void getElevation(MapGenJob &j);
    void fillChunk(MapGenJob &j);
    void addTrees(MapGenJob &j);

    void addBlock(glm::vec3 lp, int block, MapGenJob &j);

    unsigned int seed;

    int AIR = 0;
    int GRASS_BLOCK = 0, DIRT_BLOCK = 0, STONE_BLOCK = 0;
    int PLANT_STEM_BLOCK = 0, LEAVES_BLOCK = 0;
    int TALLGRASSES[6];
    int FLOWERS[8];

    module::Perlin terrainGeneralElevation;
    module::RidgedMulti terrainMountains;
    module::Billow terrainFlatBase;
    module::ScaleBias terrainFlat;
    module::Perlin terrainType;
    module::Select terrainPreElevation;
    module::Add terrainFinal;

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