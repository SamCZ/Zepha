#pragma once

#include "client/menu/SubgameConfig.h"

class AtlasTexture;

class SubgameDef {
public:
	SubgameDef(const SubgameConfig& config, const sptr<AtlasTexture>& icon, const std::filesystem::path& path):
		config(config), icon(icon), path(path) {}
		
	SubgameConfig config {};
	sptr<AtlasTexture> icon;
	std::filesystem::path path;
};
