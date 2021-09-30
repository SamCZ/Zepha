#pragma once

#include "MeshPart.h"
#include "util/Types.h"

struct BlockModel {
	BlockModel() = default;
	explicit BlockModel(vec<std::tuple<AtlasTexture, optional<u32>, optional<AtlasTexture>>> data);
	
	vec<AtlasTexture> textures {};
	array<vec<MeshPart>, 7> parts {};
	vec<std::pair<MeshMod, f32>> meshMods {};
	
	bool culls = false;
	bool visible = false;
};