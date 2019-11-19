//
// Created by aurailus on 28/01/19.
//

#pragma once


#include <glm/vec3.hpp>
#include <vector>
#include <noise/noise.h>

#include "MapGenJob.h"
#include "../LocalDefs.h"
#include "BiomeAtlas.h"
#include "../../world/chunk/BlockChunk.h"

using namespace noise;

class MapGen {
public:
    MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biome);
    BlockChunk* generate(glm::vec3 pos);
private:
    void getDensityMap(MapGenJob &job);
    void getElevation(MapGenJob &j);
    void fillChunk(MapGenJob &j);
//    void addTrees(MapGenJob &j);

//    void addBlock(glm::vec3 lp, unsigned int block, MapGenJob &j);

    unsigned int seed = 0;

    DefinitionAtlas& atlas;
    BiomeAtlas& biomes;

    module::Perlin temperatureBase;
    module::ScaleBias temperature;

    module::Perlin humidityBase;
    module::ScaleBias humidity;

    module::Perlin roughnessBase;
    module::ScaleBias roughness;

    //
    //  OLD PARAMETERS
    //

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
