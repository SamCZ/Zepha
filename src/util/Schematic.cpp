//
// Created by aurailus on 2020-02-01.
//

#include "Schematic.h"

#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"

void Schematic::process(DefinitionAtlas& atlas) {
	blocks.reserve(stringData.size());
	
	for (auto& string : stringData) blocks.push_back(atlas.blockFromStr(string).index);
	
	stringData.clear();
	stringData.shrink_to_fit();
	
	processed = true;
}

glm::ivec3 Schematic::getOffset(unsigned int ind) {
	glm::ivec3 vec{};
	
	vec.z = ind / (dimensions.x * dimensions.y);
	ind -= (vec.z * dimensions.x * dimensions.y);
	vec.y = ind / dimensions.z;
	vec.x = ind % dimensions.x;
	
	return vec;
}

unsigned int Schematic::index(const glm::ivec3& vec) {
	return static_cast<unsigned int>(vec.x + dimensions.x * (vec.y + dimensions.y * vec.z));
}