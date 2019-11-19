//
// Created by aurailus on 2019-11-18.
//

#include "LocalBiomeAtlas.h"

LocalBiomeAtlas::LocalBiomeAtlas() {
    //Invalid Biome
    BiomeDef* invalid = new BiomeDef("invalid", 0, -1, -1, -1, 0, 0, 0, {});
    defs.push_back(invalid);
    defTable.insert({"invalid", 0});
}

void LocalBiomeAtlas::setIdentifiers(std::vector<std::string> &identifiers) {
    for (unsigned int i = 0; i < identifiers.size(); i++) {
        if (i >= 2) defs.emplace_back(); //Don't emplace invalid
        defTable.insert({identifiers[i], i});
    }
}

void LocalBiomeAtlas::registerBiome(BiomeDef *def) {
    if (!defTable.count(def->identifier)) {
        std::cout << Log::err << "Client/Server biome identifier desync: " + def->identifier + ". Exiting." << Log::endl;
        exit(1);
    }
    def->index = defTable[def->identifier];
    defs[def->index] = def;
}
