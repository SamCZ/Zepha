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
#include "../../util/Vec.h"

class World;
class Chunk;
class Subgame;
class BiomeAtlas;
class DefinitionAtlas;

class MapGen {
public:
    constexpr static unsigned int BIOP = 4; // Biome Prop Precision
    constexpr static unsigned int TERP = 4; // Terrain Map Precision

    typedef std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>, Vec::ivec3> ChunkMap;
    typedef std::unordered_set<glm::ivec3, Vec::ivec3> CreatedSet;

    struct Job {
        Job(glm::ivec3 pos, unsigned int size) :
            pos(pos), size(size),
            volume {{size * TERP, (size + 1) * TERP}, {1, 1.25}}, heightmap {{size * TERP, 0}},
            temperature {{size * BIOP, 0}}, roughness {{size * BIOP, 0}}, humidity {{size * BIOP, 0}} {}

        glm::ivec3 pos {};
        unsigned int size {};

        std::unique_ptr<ChunkMap> chunks = std::make_unique<ChunkMap>();

        NoiseSample volume, heightmap;
        NoiseSample temperature, humidity, roughness;
    };

    typedef std::array<float, 4096> ChunkData;

	MapGen(Subgame& game, World& world, unsigned int seed);

    std::unique_ptr<CreatedSet> generateChunk(unsigned int dim, glm::ivec3 pos);
    std::unique_ptr<CreatedSet> generateMapBlock(unsigned int dim, glm::ivec3 pos);
    std::unique_ptr<CreatedSet> generateArea(unsigned int dim, glm::ivec3 origin, unsigned int size = 1);
private:

    static std::unique_ptr<ChunkData> populateChunkDensity(Job& job, glm::ivec3 localPos);
    static std::unique_ptr<ChunkData> populateChunkDepth(Job& job, std::unique_ptr<ChunkData>& chunkDensity, std::unique_ptr<ChunkData> chunkDensityAbove);

    void generateChunkBlocks(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap);
    void generateChunkStructures(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap);

    static void setBlock(Job& job, glm::ivec3 worldPos, unsigned int block, std::shared_ptr<Chunk> hint);

//	// Generate sunlight on the mapgen threads to speed up perf
//    void generateSunlight(ChunkMap& chunks, glm::ivec3 mbPos);
//    static bool containsWorldPos(Chunk *chunk, glm::ivec3 pos);
//    void propogateSunlightNodes(ChunkMap& chunks, std::queue<SunlightNode>& queue);

    MapGenProps props;
	unsigned int seed = 0;

	Subgame& game;
	World& world;
};