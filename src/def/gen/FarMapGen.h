//
// Created by aurailus on 2020-04-05.
//

#pragma once

#include "FarMapJob.h"
#include "BiomeAtlas.h"
#include "NoiseSample.h"
#include "MapGenProps.h"
#include "../../util/Vec.h"
#include "../DefinitionAtlas.h"
#include "../../game/scene/world/graph/MeshFarMap.h"

class FarMapGen {
    FarMapGen(unsigned int seed, DefinitionAtlas& defs, BiomeAtlas& biomes, std::shared_ptr<MapGenProps> props);

    MeshFarMap generateFarMap(glm::ivec3 mbPos, unsigned short downScale);
private:
    // Build the density map for a job.
    void buildDensityMap(FarMapJob* job, glm::ivec3 mbPos);

    // Build the elevation map for a job.
    void buildElevationMap(FarMapJob& job);

    // Generate blocks and structures on a chunk, respectively. generateStructures can create partials.
//    void generateBlocks(chunk_partial& chunk);
//    void generateStructures(chunk_partials_map& chunks, chunk_partial& chunk);

    unsigned int seed = 0;

    DefinitionAtlas& defs;
    BiomeAtlas& biomes;

    std::shared_ptr<MapGenProps> props;
};
