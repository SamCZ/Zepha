//
// Created by aurailus on 05/08/19.
//

#pragma once

#include <vector>

#include "ServerTexture.h"
#include "SerializedModel.h"

class AssetStorage {
	public:
	std::vector<ServerTexture> textures;
	std::vector<SerializedModel> models;
};

