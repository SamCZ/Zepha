#pragma once

#include "client/menu/SubgameConfig.h"
#include "game/atlas/asset/AtlasTexture.h"

class SubgameDef {
public:
	AtlasTexture iconRef;
	SubgameConfig config {};
	std::filesystem::path subgamePath;
};
