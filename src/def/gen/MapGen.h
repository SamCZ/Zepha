//
// Generates terrain in the form of MapBlocks.
// Created by aurailus on 28/01/19.
//

#pragma once

#include <queue>
#include <memory>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../util/Vec.h"

class Chunk;
class MapGenJob;
class BiomeAtlas;
class MapGenProps;
class DefinitionAtlas;

class MapGen {
public:
    typedef std::pair<MapGenJob*, Chunk*> chunk_partial;
    typedef std::unordered_map<glm::ivec3, chunk_partial, Vec::ivec3> chunk_partials_map;

	MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biome, std::shared_ptr<MapGenProps> props);
    chunk_partials_map generateMapBlock(glm::ivec3 mbPos);

    // Combine two chunk partials, or a chunk and a chunk partial.
    // If both are partials `b` takes preference, if one is a fully generated chunk the partial takes preference.
    static std::shared_ptr<Chunk> combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b);
private:
    struct SunlightNode {
        SunlightNode(unsigned short index, Chunk* chunk) : index(index), chunk(chunk) {};
        unsigned short index;
        Chunk* chunk;
    };

    // Generate a chunk at `worldPos`, and place it and any partials in `chunks`.
	void generateChunk(chunk_partials_map& chunks, glm::ivec3 worldPos);

    // Build the density map for a job.
	void buildDensityMap(MapGenJob* job, glm::ivec3 worldPos);

	// Build the elevation map for a chunk, which uses the `chunks` array for efficiency.
	void buildElevationMap(chunk_partials_map& chunks, chunk_partial& chunk);

	// Generate blocks and structures on a chunk, respectively. generateStructures can create partials.
	void generateBlocks(chunk_partial& chunk);
	void generateStructures(chunk_partials_map& chunks, chunk_partial& chunk);

	// Generate sunlight on the mapgen threads to speed up perf
    void generateSunlight(chunk_partials_map& chunks, glm::ivec3 mbPos);
    static bool containsWorldPos(Chunk *chunk, glm::ivec3 pos);
    void propogateSunlightNodes(chunk_partials_map& chunks, std::queue<SunlightNode>& queue);

	// Place block in the `chunks` array, creates a partial if necessary.
    static void setBlock(glm::ivec3 worldPos, unsigned int block, chunk_partials_map& chunks);

	unsigned int seed = 0;

	DefinitionAtlas& defs;
	BiomeAtlas& biomes;

    std::shared_ptr<MapGenProps> props;
};