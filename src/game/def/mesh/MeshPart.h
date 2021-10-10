#pragma once

#include "util/Types.h"
#include "game/def/mesh/ShaderMod.h"
#include "game/def/mesh/BlockModelVertex.h"

class AtlasTexture;

struct MeshPart {
	MeshPart(const vec<BlockModelVertex>& vertices, const vec<u32>& indices,
		optional<sptr<AtlasTexture>> texture = std::nullopt, optional<u32> blendInd = std::nullopt,
		optional<sptr<AtlasTexture>> blendTexture = std::nullopt);
	
	vec<BlockModelVertex> vertices;
	vec<u32> indices;
	
	optional<u32> blendInd;
	optional<sptr<AtlasTexture>> texture;
	optional<sptr<AtlasTexture>> blendTexture;
	
	ShaderMod shaderMod = ShaderMod::NONE;
	f32 modValue = 0;
};