//
// Created by aurailus on 2019-11-13.
//

#include "BiomeAtlas.h"

#include "game/def/BiomeDef.h"

unsigned int BiomeAtlas::size() {
    return defs.size();
}

BiomeDef& BiomeAtlas::biomeFromId(unsigned int index) {
    if (index >= defs.size()) throw std::runtime_error("Undefined biome ID " + std::to_string(index) + " requested.");
    return *defs.at(static_cast<unsigned long>(index));
}

BiomeDef& BiomeAtlas::biomeFromStr(const std::string& identifier) {
    if (defTable.count(identifier) <= 0) throw std::runtime_error("Undefined biome identifier " + identifier + " requested.");
    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}

std::vector<BiomeDef*> BiomeAtlas::biomesFromTag(const std::string& tag) {
	std::vector<BiomeDef*> matched {};
	for (auto biome : defs)
		if (biome->tags.count(tag) && biome->tags.at(tag) != 0) matched.push_back(biome);
	return std::move(matched);
}
