#include "BiomeAtlas.h"

#include "game/def/BiomeDef.h"

u16 BiomeAtlas::size() {
	return defs.size();
}

BiomeDef& BiomeAtlas::biomeFromId(u16 index) {
	if (index >= defs.size()) throw std::runtime_error(
		"Undefined biome ID " + std::to_string(index) + " requested.");
	return *defs.at(index);
}

BiomeDef& BiomeAtlas::biomeFromStr(const string& identifier) {
	if (defTable.count(identifier) <= 0) throw std::runtime_error(
		"Undefined biome identifier " + identifier + " requested.");
	return *defs.at(defTable.at(identifier));
}

vec<BiomeDef*> BiomeAtlas::biomesFromTag(const string& tag) {
	vec<BiomeDef*> matched {};
	for (auto biome : defs)
		if (biome->tags.count(tag) && biome->tags.at(tag) != 0) matched.push_back(biome);
	return std::move(matched);
}
