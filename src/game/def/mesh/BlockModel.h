#pragma once

#include "MeshPart.h"
#include "util/Types.h"

class AtlasTexture;

struct BlockModel {
	BlockModel() = default;
	explicit BlockModel(vec<std::tuple<sptr<AtlasTexture>, optional<u32>, optional<sptr<AtlasTexture>>>> data);
	
	array<vec<MeshPart>, 7> parts {};
	vec<sptr<AtlasTexture>> textures {};
	vec<std::pair<MeshMod, f32>> meshMods {};
	
	bool culls = false;
	bool visible = false;
};