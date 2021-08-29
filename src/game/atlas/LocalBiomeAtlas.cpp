//
// Created by aurailus on 2019-11-18.
//

#include "LocalBiomeAtlas.h"

#include <stdexcept>

#include "game/def/BiomeDef.h"

LocalBiomeAtlas::LocalBiomeAtlas() {
	BiomeDef* invalid = new BiomeDef();
	invalid->identifier = "invalid";
	invalid->humidity = -1;
	invalid->temperature = -1;
	invalid->roughness = -1;
	
	defs.push_back(invalid);
	defTable.insert({ "invalid", 0 });
}

void LocalBiomeAtlas::setIdentifiers(const std::vector<std::string>& identifiers) {
	for (unsigned int i = 0; i < identifiers.size(); i++) {
		if (i >= 1) defs.emplace_back(); //Don't emplace invalid
		defTable.insert({ identifiers[i], i });
	}
}

void LocalBiomeAtlas::registerBiome(BiomeDef* def) {
	if (!defTable.count(def->identifier))
		throw std::runtime_error("Client/Server biome desync: " + def->identifier + ".");
	def->index = defTable[def->identifier];
	defs[def->index] = def;
}
