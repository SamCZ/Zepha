#include "ServerBiomeAtlas.h"

#include "game/def/BiomeDef.h"

ServerBiomeAtlas::ServerBiomeAtlas(u32 seed) :
	seed(seed) {
	
	BiomeDef* invalid = new BiomeDef();
	invalid->identifier = "invalid";
	invalid->humidity = -1;
	invalid->temperature = -1;
	invalid->roughness = -1;
	
	registerBiome(invalid);
}

void ServerBiomeAtlas::registerBiome(BiomeDef* def) {
	defs.push_back(def);
	defTable.emplace(def->identifier, def->index);
}

u32 ServerBiomeAtlas::getSeed() {
	return seed;
}
