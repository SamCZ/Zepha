//
// Created by aurailus on 01/12/18.
//

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/normal.hpp>

#include "ChunkMeshGenerator.h"

#include "util/Vec.h"
#include "util/Util.h"
#include "util/Timer.h"
#include "client/stream/ChunkMeshDetails.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "world/gen/NoiseSample.h"
#include "world/dim/chunk/Chunk.h"
#include "game/def/mesh/BlockModel.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "game/atlas/LocalDefinitionAtlas.h"

ChunkMeshGenerator::ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs,
	LocalBiomeAtlas& biomes,
	std::unique_ptr<Chunk> chunk, std::array<std::unique_ptr<Chunk>, 6> adjacent,
	std::array<NoiseSample, 3>& blockOffsets) :
	meshDetails(meshDetails),
	adjacent(std::move(adjacent)),
	biomes(biomes),
	chunk(std::move(chunk)),
	defs(defs) {
	
	Timer t("Mesh generation");
	
	meshDetails->vertices.reserve(5000);
	meshDetails->indices.reserve(7000);
	
	BlockDef* block = nullptr;
	BiomeDef* biome = nullptr;
	
	for (unsigned short i = 0; i < 4096; i++) {
		if (!block || block->index != chunk->getBlocksArray()[i])
			block = &defs.blockFromId(chunk->getBlocksArray()[i]);
		if (!biome || biome->index != chunk->getBiomesArray()[i])
			biome = &biomes.biomeFromId(chunk->getBiomesArray()[i]);
		
		BlockModel& model = block->model;
		glm::vec3 biomeTint = biome->tint;
		
		if (!model.visible) continue;
		
		glm::ivec3 off = Space::Block::fromIndex(i);
		glm::vec3 vis = off;
		
		for (auto& mod : model.meshMods) {
			switch (mod.first) {
			default: break;
			case MeshMod::OFFSET_X: vis.x += blockOffsets[0].get(glm::vec3(off) / 16.f) * mod.second;
				break;
			case MeshMod::OFFSET_Y: vis.y += blockOffsets[1].get(glm::vec3(off) / 16.f) * mod.second;
				break;
			case MeshMod::OFFSET_Z: vis.z += blockOffsets[2].get(glm::vec3(off) / 16.f) * mod.second;
				break;
			}
		}
		
		for (unsigned char j = 0; j < 6; j++) {
			const auto pos = off + Vec::TO_VEC[j];
			const auto blockInd = getBlockAt(pos);
			const auto* blockDef = block->index == blockInd ? block : &defs.blockFromId(blockInd);
			
			if (!blockDef->culls) addFaces(vis, model.parts[j], biomeTint, getLightAt(pos));
		}
		
		addFaces(vis, model.parts[static_cast<int>(EVec::NO_CULL)], biomeTint, getLightAt(off));
	}
	
	meshDetails->vertices.shrink_to_fit();
	meshDetails->indices.shrink_to_fit();
}

unsigned int ChunkMeshGenerator::getBlockAt(const glm::ivec3& pos) {
	auto dir = glm::floor(glm::vec3(pos) / 16.f);
	if (dir.x != 0 || dir.y != 0 || dir.z != 0) {
		unsigned int ind = static_cast<unsigned int>(Vec::TO_ENUM.at(dir));
		return adjacent[ind]->getBlock(Space::Block::index(pos));
	}
	return chunk->getBlocksArray()[Space::Block::index(pos)];
}

glm::vec4 ChunkMeshGenerator::getLightAt(const glm::ivec3& pos) {
	auto dir = glm::floor(glm::vec3(pos) / 16.f);
	if (dir.x != 0 || dir.y != 0 || dir.z != 0) {
		unsigned int ind = static_cast<unsigned int>(Vec::TO_ENUM.at(dir));
		return adjacent[ind]->getLight(Space::Block::index(pos));
	}
	
	return chunk->getLight(Space::Block::index(pos));
}

void ChunkMeshGenerator::addFaces(const glm::vec3& offset, const std::vector<MeshPart>& meshParts,
	const glm::vec3& tint, glm::vec4 light) {
	
	for (const MeshPart& mp : meshParts) {
		glm::vec3 modData = {};
		
		switch (mp.shaderMod) {
		default: break;
		
		case ShaderMod::ROTATE_X:
		case ShaderMod::ROTATE_Y:
		case ShaderMod::ROTATE_Z:
		case ShaderMod::SWAY_ATTACHED:
		case ShaderMod::SWAY_FULL_BLOCK:
			modData = { Util::packFloat((offset - 8.f) / 8.f), mp.modValue, 0 };
			break;
		}
		
		for (const BlockModelVertex& vertex : mp.vertices) {
			meshDetails->vertices.push_back({
				vertex.pos + offset,
				vertex.tex,
				mp.blendInd ? tint : glm::vec3 { 1, 1, 1 },
				mp.blendInd ? vertex.blendMask : glm::vec2 { -1, -1 },
				Util::packFloat(vertex.nml),
				glm::vec4(light),
				static_cast<float>(mp.shaderMod),
				modData
			});
		}
		
		for (unsigned int index : mp.indices)
			meshDetails->indices.push_back(indOffset + index);
		
		indOffset += mp.vertices.size();
	}
}