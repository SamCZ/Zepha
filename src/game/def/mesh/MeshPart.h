#pragma once

#include "util/Types.h"
#include "game/def/mesh/ShaderMod.h"
#include "game/atlas/asset/AtlasTexture.h"
#include "game/def/mesh/BlockModelVertex.h"

struct MeshPart {
	MeshPart(const vec<BlockModelVertex>& vertices, const vec<u32>& indices,
		optional<AtlasTexture> texture = std::nullopt, optional<u32> blendInd = std::nullopt,
		optional<AtlasTexture> blendTexture = std::nullopt);
	
	vec<BlockModelVertex> vertices;
	vec<u32> indices;
	
	optional<u32> blendInd;
	optional<AtlasTexture> texture;
	optional<AtlasTexture> blendTexture;
	
	ShaderMod shaderMod = ShaderMod::NONE;
	f32 modValue = 0;
};