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
    typedef std::pair<MapGenJob*, BlockChunk*> chunk_partial;
    typedef std::unordered_map<glm::ivec3, chunk_partial, Vec::ivec3> chunk_partials_map;

	MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biome);
    chunk_partials_map generateMapBlock(glm::ivec3 mbPos);
private:
    // Generate a chunk at `worldPos`, and place it & any partials in `chunks`.
	void generateChunk(chunk_partials_map& chunks, glm::ivec3 worldPos);

    // Build the density map for a job.
	void buildDensityMap(MapGenJob* job, glm::ivec3 worldPos);

	// Build the elevation map for a chunk, which uses the `chunks` partials array for efficiency.
	void buildElevationMap(chunk_partials_map& chunks, chunk_partial& chunk);

	// Fill a chunk with blocks and any structures that should be included, may generate partials.
	// Returns chunks in the `chunk` vector.
	void fillChunkBlocks(chunk_partial& chunk);

	// Fill a chunk with structures
	void fillChunkStructures(chunk_partial& chunk);

	unsigned int seed = 0;

	DefinitionAtlas& defs;
	BiomeAtlas& biomes;

    module::Perlin temperatureBase;
    module::Turbulence temperatureTurbulence;
	module::ScaleBias temperature;

	module::Perlin humidityBase;
    module::Turbulence humidityTurbulence;
	module::ScaleBias humidity;

	module::Perlin roughnessBase;
    module::Turbulence roughnessTurbulence;
	module::ScaleBias roughness;

	module::Perlin treeMap;
	module::Abs treeAbs;
};