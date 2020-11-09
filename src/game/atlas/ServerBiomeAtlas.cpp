//
// Created by aurailus on 2019-11-18.
//

#include "ServerBiomeAtlas.h"

#include "game/def/BiomeDef.h"

ServerBiomeAtlas::ServerBiomeAtlas(unsigned int seed) :
    seed(seed) {
    
	BiomeDef* invalid = new BiomeDef();
	invalid->identifier = "invalid";
	invalid->humidity = -1;
	invalid->temperature = -1;
	invalid->roughness = -1;
	
    registerBiome(invalid);
}

void ServerBiomeAtlas::registerBiome(BiomeDef *def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}
