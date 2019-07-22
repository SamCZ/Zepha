//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockAtlas.h"

LocalBlockAtlas::LocalBlockAtlas() {
    //Register Air Node
    LocalBlockModel nullModel; nullModel.visible = false, nullModel.culls = false;
    LocalBlockDef air("builtin:air", nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    air.index = 0;
    definitions[0] = air;
}

void LocalBlockAtlas::setIdentifiers(std::vector<std::string> &identifiers) {
    for (int i = 0; i < identifiers.size(); i++) {
        definitions.emplace_back();
        identifierIndexTable.insert({identifiers[i], i});
    }
}

void LocalBlockAtlas::registerBlock(LocalBlockDef def) {
    if (!identifierIndexTable.count(def.identifier)) {
        std::cout << Log::err << "Client/Server block identifier desync: " + def.identifier + "!" << Log::endl;
        return;
    }

    def.index = identifierIndexTable[def.identifier];
    definitions[def.index] = def;
}

LocalBlockDef& LocalBlockAtlas::fromIndex(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at(static_cast<unsigned long>(id));
    }

    std::cout << Log::err << "Block ID #" << id << " (undefined) requested! Returning air." << Log::endl;
    return definitions.at(0);
}

LocalBlockDef &LocalBlockAtlas::fromIdentifier(std::string identifier) {
    if (identifierIndexTable.count(identifier) > 0) {
        return definitions.at(static_cast<unsigned long>(identifierIndexTable.at(identifier)));
    }

    std::cout << Log::err << "Block Identifier \"" << identifier << "\" (undefined) requested! Returning air." << Log::endl;
    return definitions.at(0);
}