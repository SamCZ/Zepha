//
// Created by aurailus on 28/01/19.
//

#ifndef ZEUS_MAPGEN_H
#define ZEUS_MAPGEN_H


#include <glm/vec3.hpp>
#include <vector>

#include "MapGenJob.h"
#include "../LocalDefs.h"
#include "../../world/chunk/BlockChunk.h"

#include <noise/noise.h>

using namespace noise;

class MapGen {
public:
    MapGen() = default;
    MapGen(unsigned int seed, BlockAtlas& atlas);
    BlockChunk* generate(glm::vec3 pos);
private:
    void getDensityMap(MapGenJob &job);
    void getElevation(MapGenJob &j);
    void fillChunk(MapGenJob &j);
    void addTrees(MapGenJob &j);

    void addBlock(glm::vec3 lp, unsigned int block, MapGenJob &j);

    unsigned int seed = 0;

    unsigned int AIR = 0;
    unsigned int GRASS_BLOCK = 0, DIRT_BLOCK = 0, STONE_BLOCK = 0;
    unsigned int PLANT_STEM_BLOCK = 0, LEAVES_BLOCK = 0;
    unsigned int TALLGRASSES[6] = {0, 0, 0, 0, 0, 0};
    unsigned int FLOWERS[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    //Smooth world noise factors
    module::Perlin worldElevationBase;
    module::ScaleBias worldElevationScaled;

    module::Perlin worldFeatureBase;
    module::ScaleBias worldFeatureScaled;

    module::Add worldSmoothElevation;

    //Mountain hold
    module::Perlin mountainMultiplierBase;
    module::ScaleBias mountainMultiplierScaled;
    module::Clamp mountainMultiplierClamped;

    //Mountain noise types
    module::Perlin mountainSmoothBase;
    module::ScaleBias mountainSmoothScaled;

    module::RidgedMulti mountainRoughBase;
    module::ScaleBias mountainRoughScaled;
    module::Perlin mountainRoughHoldBase;
    module::ScaleBias mountainRoughHoldScaled;

    module::Multiply mountainRoughMultiplied;

    module::Add mountainNoise;

//    module::Perlin mountainRoughnessMultiplierBase;
//    module::ScaleBias mountainRoughnessMultiplierScaled;
//
//    module::RidgedMulti terrainMountainElevation;
//    module::ScaleBias terrainMountainElevationScaled;
//
//    module::Perlin terrainMountainHold;
//    module::ScaleBias terrainMountainHoldScaled;
//    module::Multiply terrainMountainMultiplied;
//
//    module::Add terrainElevation;

    module::Multiply mountainMultiplied;

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