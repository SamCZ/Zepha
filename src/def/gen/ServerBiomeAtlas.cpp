//
// Created by aurailus on 2019-11-18.
//

#include "ServerBiomeAtlas.h"

ServerBiomeAtlas::ServerBiomeAtlas() {
    //Invalid Biome
    BiomeDef* invalid = new BiomeDef("invalid", INVALID, -1, -1, -1, 0, 0, 0, {}, {}, {});
    registerBiome(invalid);
}

void ServerBiomeAtlas::registerBiome(BiomeDef *def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}
