//
// Created by aurailus on 28/01/19.
//

#pragma once

#include "MapGenJob.h"
#include "../../util/Vec.h"
#include "../../world/chunk/BlockChunk.h"

using namespace noise;

class MapGen {
public:
    typedef std::pair<MapGenJob*, BlockChunk*> chunk_partial;
    typedef std::unordered_map<glm::ivec3, chunk_partial, Vec::ivec3> chunk_partials_map;

	MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biome);
    chunk_partials_map generateMapBlock(glm::ivec3 mbPos);

    // Combine two chunk partials, or a chunk and a chunk partial.
    // If both are partials `b` takes preference, if one is a fully generated chunk the partial takes preference.
    static std::shared_ptr<BlockChunk> combinePartials(std::shared_ptr<BlockChunk> a, std::shared_ptr<BlockChunk> b);
private:
    // Generate a chunk at `worldPos`, and place it and any partials in `chunks`.
	void generateChunk(chunk_partials_map& chunks, glm::ivec3 worldPos);

    // Build the density map for a job.
	void buildDensityMap(MapGenJob* job, glm::ivec3 worldPos);

	// Build the elevation map for a chunk, which uses the `chunks` array for efficiency.
	void buildElevationMap(chunk_partials_map& chunks, chunk_partial& chunk);

	// Generate blocks and structures on a chunk, respectively. generateStructures can create partials.
	void generateBlocks(chunk_partial& chunk);
	void generateStructures(chunk_partials_map& chunks, chunk_partial& chunk);

	// Place block in the `chunks` array, creates a partial if necessary.
    static void setBlock(glm::ivec3 worldPos, unsigned int block, chunk_partials_map& chunks);

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
};