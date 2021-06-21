//
// Created by aurailus on 01/12/18.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>

class Chunk;
class MeshPart;
class BlockDef;
class NoiseSample;
class LocalBiomeAtlas;
class ChunkMeshDetails;
class LocalDefinitionAtlas;

class ChunkMeshGenerator {
public:
	ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
		std::unique_ptr<Chunk> chunk, std::array<std::unique_ptr<Chunk>, 6> adjacent,
		std::array<NoiseSample, 3>& blockOffsets);
	
private:
	inline unsigned int getBlockAt(const glm::ivec3& pos);
	inline glm::vec4 getLightAt(const glm::ivec3& pos);
	
	void
	addFaces(const glm::vec3& offset, const std::vector<MeshPart>& meshParts, const glm::vec3& tint, glm::vec4 light);
	
	LocalDefinitionAtlas& defs;
	LocalBiomeAtlas& biomes;
	
	unsigned int indOffset = 0;
	ChunkMeshDetails* meshDetails;
	
	std::unique_ptr<Chunk> chunk;
	std::array<std::unique_ptr<Chunk>, 6> adjacent;
};