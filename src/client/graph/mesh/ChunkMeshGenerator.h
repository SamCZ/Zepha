#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include "util/Types.h"

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
		uptr<Chunk> chunk, array<uptr<Chunk>, 6> adjacent, array<NoiseSample, 3>& blockOffsets);
	
private:
	inline u16 getBlockAt(const ivec3& pos);
	inline u8vec4 getLightAt(const ivec3& pos);
	
	void addFaces(const vec3& offset, const vec<MeshPart>& meshParts, const vec3& tint, u8vec4 light);
	
	LocalDefinitionAtlas& defs;
	LocalBiomeAtlas& biomes;
	
	usize indOffset = 0;
	ChunkMeshDetails* meshDetails;
	
	uptr<Chunk> chunk;
	array<uptr<Chunk>, 6> adjacent;
};