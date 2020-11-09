//
// Generates terrain in the form of MapBlocks.
// Created by aurailus on 28/01/19.
//

#pragma once

#include <queue>
#include <memory>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <unordered_set>

#include "MapGenProps.h"
#include "NoiseSample.h"
#include "util/Vec.h"
#include "util/Voronoi3D.h"

class World;
class Chunk;
class Subgame;
class BiomeDef;
class BiomeAtlas;
class DefinitionAtlas;

class MapGen {
public:
	/** The precision of the Biome map, as a divisor of the chunk size. */
	constexpr static unsigned int BIOP = 4;
	
	/** The precision of the Terrain maps, as a divisor of the chunk size. */
	constexpr static unsigned int TERP = 4;
	
	/** A type alias for the returned list of chunk positions that were generated. */
	typedef std::unordered_set<glm::ivec3, Vec::ivec3> CreatedSet;
	
	/**
	 * A struct containing all the information for a generation job.
	 * Contains a list of chunks, Noise samples, and the world position of the the job's root.
	 */
	
	struct Job {
		/** A type alias for the type the map of Chunks stored in the Job. */
		typedef std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, Vec::ivec3> ChunkMap;
		
		/**
		 * Creates a new job with the root position and size specified, and initializes the NoiseSample params.
		 * @param pos - The root position of the job.
		 * @param size - The size in chunks of the job.
		 */
		
		Job(glm::ivec3 pos, unsigned int size) :
			pos(pos), size(size),
			volume {{ size * TERP, (size + 1) * TERP }, { 1, 1.25 }}, heightmap {{ size * TERP, 0 }},
			temperature {{ size * BIOP, 0 }}, roughness {{ size * BIOP, 0 }}, humidity {{ size * BIOP, 0 }} {}
		
		glm::ivec3 pos {};
		unsigned int size {};
		
		std::unique_ptr<ChunkMap> chunks = std::make_unique<ChunkMap>();
		
		NoiseSample volume, heightmap;
		NoiseSample temperature, humidity, roughness;
	};
	
	typedef std::array<float, 4096> ChunkData;
	
	MapGen(const MapGen& o) = delete;
	
	/**
	 * Create a MapGen object with the seed and biomes provided.
	 *
	 * @param game - A reference to the subgame, for block and biome definitions.
	 * @param world - A reference to the world, for assigning the blocks to.
	 * @param seed - A seed to base the generation off of.
	 * @param biomes - A list of biome identifiers or tags to include in generation.
	 */
	
	MapGen(Subgame& game, World& world, unsigned int seed, std::unordered_set<std::string> biomes);
	
	/**
	 * Generate a single chunk at the dimension and position provided.
	 * As with all generate* functions, this may result in extraneous chunk partials being created.
	 * This is the least efficient generation call and should be used only in extenuating circumstances.
	 *
	 * @param dim - The dimension to insert the finished chunk into.
	 * @param pos - The position in the dimension to generate the chunk.
	 * @returns a set of positions that were generated by this function call.
	 */
	
	[[maybe_unused]] std::unique_ptr<CreatedSet> generateChunk(unsigned int dim, glm::ivec3 pos);
	
	/**
	 * Generate a mapblock at the dimension and position provided.
	 * As with all generate* functions, this may result in extraneous chunk partials being created.
	 * Commonly used by the ServerGenStream for regular chunk generation.
	 *
	 * @param dim - The dimension to insert the finished chunk into.
	 * @param pos - The position in the dimension to generate the chunk.
	 * @returns a set of positions that were generated by this function call.
	 */
	
	std::unique_ptr<CreatedSet> generateMapBlock(unsigned int dim, glm::ivec3 pos);
	
	/**
	 * The underlying generate function called by both generateMapBlock and generateChunk.
	 * Can also be called on it's own to generate an arbitrary region of chunks.
	 * As with all generate* functions, this may result in extraneous chunk partials being created.
	 *
	 * @param dim - The dimension to insert the finished chunk into.
	 * @param pos - The position in the dimension to generate the chunk.
	 * @return - A set of positions that were generated by this function call.
	 */
	
	std::unique_ptr<CreatedSet> generateArea(unsigned int dim, glm::ivec3 origin, unsigned int size = 1);

private:
	
	/**
	 * Get the closest biome to the provided environmental values from the Vonoroi map.
	 * Returns the index of the matched biome.
	 *
	 * @param temperature - The temperature value of the position to check.
	 * @param humidity - The humidity value of the position to check.
	 * @param roughness  - The roughness value of the position to check.
	 * @returns the biome index of the environmentally closest biome.
	 */
	
	unsigned int getBiomeAt(float temperature, float humidity, float roughness);
	
	/**
	 * Generate the Vonoroi biome map, using the biomes listed,
	 * according to their definition parameters.
	 *
	 * @param biomes - The biomes to add to the map.
	 */
	
	void generateVoronoi(const std::unordered_set<unsigned int>& biomes);
	
	/**
	 * Create a density array for a chunk using a generation Job and an offset within it.
	 * Returns a flattened array of block densities for every point in the chunk.
	 *
	 * @param job - The Job to pull the data from.
	 * @param localPos - The offset of the chunk's data within the job.
	 * @returns a ChunkData array containing the chunk's density.
	 */
	
	static std::unique_ptr<ChunkData> populateChunkDensity(Job& job, glm::ivec3 localPos);
	
	/**
	 * Create a depth array for a chunk using a generation Job and the chunk densities around it.
	 * Returns a flattened array of block depths for every point in the chunk.
	 *
	 * @param chunkDensity - A density array of the current chunk.
	 * @param chunkDensityAbove - A density array of the chunk above it.
	 * @returns a ChunkData array containing the chunk's depth.
	 */
	
	static std::unique_ptr<ChunkData> populateChunkDepth(std::unique_ptr<ChunkData>& chunkDensity,
		std::unique_ptr<ChunkData> chunkDensityAbove);
	
	/**
	 * Generates a chunk's blocks from a generation Job and an offset within it, and inserts it into the Job.
	 * Combines with any partials that have been previously created at that position within job.
	 *
	 * Declares the chunk generated, and counts renderable blocks within it. Though later functions manipulate
	 * chunk data, they do so using the chunk setBlock method, making it safe to count at this stage.
	 *
	 * @param job - The Job to pull the data from.
	 * @param localPos - The offset of the chunk within the job.
	 * @param biomeMap - The two-dimensional biome array of the entire generation area.
	 * @param depthMap - The depth map of the chunk being generated.
	 */
	
	void generateChunkBlocks(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap);
	
	/**
	 * Generates structures for a Chunk based on data within the generation job and an offset within it.
	 * May create chunk partials, inserting them back into the Job for later completion.
	 *
	 * @param job - The job to pull the data from.
	 * @param localPos - The offset of the chunk within the job.
	 * @param biomeMap - The two-dimensional biome array of the entire generation area.
	 * @param depthMap - The depth map of the chunk being generated.
	 */
	
	void
	generateChunkStructures(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap);
	
	/**
	 * Sets a block at the position specified into the Job, if the block at said position is not filled by
	 * a material greater than air. If a chunk does not exist at the specified position, a partial is generated
	 * into the chunk map, and the block is assigned on there.
	 *
	 * @param job - The job to pull the data from.
	 * @param worldPos - The world position to set the block at.
	 * @param block - The block to set.
	 * @param hint - An optional parameter that may speed up the function if set to the chunk to set to.
	 */
	
	static void setBlock(Job& job, glm::ivec3 worldPos, unsigned int block, std::shared_ptr<Chunk> hint);

//	// Generate sunlight on the mapgen threads to speed up perf
//    void generateSunlight(ChunkMap& chunks, glm::ivec3 mbPos);
//    static bool containsWorldPos(Chunk *chunk, glm::ivec3 pos);
//    void propogateSunlightNodes(ChunkMap& chunks, std::queue<SunlightNode>& queue);
	
	MapGenProps props;
	unsigned int seed = 0;
	
	constexpr const static unsigned short voronoiSize = 64;
	Voronoi3D voronoi { voronoiSize };
	
	Subgame& game;
	World& world;
};