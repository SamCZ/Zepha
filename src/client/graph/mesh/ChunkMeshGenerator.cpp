#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/normal.hpp>

#include "ChunkMeshGenerator.h"

#include "util/Vec.h"
#include "util/Util.h"
#include "util/Timer.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "world/gen/NoiseSample.h"
#include "world/dim/chunk/Chunk.h"
#include "game/def/mesh/BlockModel.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "client/stream/MeshChunkDetails.h"
#include "game/atlas/LocalDefinitionAtlas.h"

ChunkMeshGenerator::ChunkMeshGenerator(MeshChunkDetails* meshDetails, LocalDefinitionAtlas& defs,
	LocalBiomeAtlas& biomes, uptr<Chunk> chk, array<uptr<Chunk>, 6> adj, array<NoiseSample, 3>& blockOffsets,
	Detail detail) :
	defs(defs),
	biomes(biomes),
	chunk(std::move(chk)),
	adjacent(std::move(adj)),
	meshDetails(meshDetails) {
	
	Timer t("Mesh generation");
	
	meshDetails->vertices.reserve(5000);
	meshDetails->indices.reserve(7000);
	
	BlockDef* block = nullptr;
	BiomeDef* biome = nullptr;
	
	auto& blocksArr = chunk->getBlocksArray();
	auto& biomesArr = chunk->getBiomesArray();
	
	for (u16 i = 0; i < 4096; i++) {
		if (!block || block->index != blocksArr[i])
			block = &defs.blockFromId(blocksArr[i]);
		if (!biome || biome->index != biomesArr[i])
			biome = &biomes.biomeFromId(biomesArr[i]);
		
		BlockModel& model = detail == Detail::HIGH ? block->model : block->farModel;
		u8vec3 biomeTint = u8vec3(biome->tint * 255.f);
		
		if (!model.visible) continue;
		
		ivec3 off = Space::Block::fromIndex(i);
		vec3 vis = off;
		
		for (auto& mod : model.meshMods) {
			switch (mod.first) {
			default: break;
			
			case MeshMod::OFFSET_X:
				vis.x += blockOffsets[0][off] * mod.second;
				break;
			
			case MeshMod::OFFSET_Y:
				vis.y += blockOffsets[1][off] * mod.second;
				break;
				
			case MeshMod::OFFSET_Z:
				vis.z += blockOffsets[2][off] * mod.second;
				break;
			}
		}
		
		for (u8 j = 0; j < 6; j++) {
			const auto pos = off + Vec::TO_VEC[j];
			const auto blockInd = getBlockAt(pos);
			const auto* blockDef = block->index == blockInd ? block : &defs.blockFromId(blockInd);
			
			if (!blockDef->culls) addFaces(vis, model.parts[j], biomeTint, getLightAt(pos));
		}
		
		addFaces(vis, model.parts[static_cast<u8>(EVec::NO_CULL)], biomeTint, getLightAt(off));
	}
	
	meshDetails->vertices.shrink_to_fit();
	meshDetails->indices.shrink_to_fit();
	
//	t.printElapsedMs();
}

u16 ChunkMeshGenerator::getBlockAt(const ivec3& pos) {
	auto dir = glm::floor(vec3(pos) / 16.f);
	if (dir.x != 0 || dir.y != 0 || dir.z != 0) {
		u8 ind = static_cast<u8>(Vec::TO_ENUM.at(dir));
		return adjacent[ind]->getBlock(Space::Block::index(pos));
	}
	return chunk->getBlocksArray()[Space::Block::index(pos)];
}

u8vec4 ChunkMeshGenerator::getLightAt(const ivec3& pos) {
//	auto dir = glm::floor(vec3(pos) / 16.f);
//	if (dir.x != 0 || dir.y != 0 || dir.z != 0) {
//		u8 ind = static_cast<u8>(Vec::TO_ENUM.at(dir));
//		return adjacent[ind]->getLight(Space::Block::index(pos));
//	}
//	return chunk->getLight(Space::Block::index(pos));
	return { 0, 0, 0, 15 };
}

void ChunkMeshGenerator::addFaces(const vec3& offset,
	const vec<MeshPart>& meshParts, const u8vec3& tint, const u8vec4& light) {
	for (const MeshPart& mp : meshParts) {
		vec3 modData = {};
		
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
			meshDetails->vertices.push_back(MeshChunk::Vertex {
				vertex.pos + offset,
				vertex.tex,
				mp.blendInd ? tint : u8vec3(255),
				mp.blendInd ? vertex.blendMask : vec2 { -1, -1 },
				Util::packFloat(vertex.nml),
				light,
				static_cast<u8>(mp.shaderMod),
				modData
			});
		}
		
		for (usize index : mp.indices) meshDetails->indices.push_back(indOffset + index);
		indOffset += mp.vertices.size();
	}
}