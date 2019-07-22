//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockAtlas.h"

LocalBlockAtlas::LocalBlockAtlas() {
    //Register Air Node
    LocalBlockModel nullModel; nullModel.visible = false, nullModel.culls = false;
    LocalBlockDef air("builtin:air", static_cast<int>(definitions.size()), nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    definitions.push_back(air);
    identifierIndexTable["builtin:air"] = AIR;
}

int LocalBlockAtlas::definitionsSize() {
    return static_cast<int>(definitions.size());
}

void LocalBlockAtlas::setIdentifiers(std::vector<std::string> &identifiers) {
    for (int i = 0; i < identifiers.size(); i++) {
        identifierIndexTable.insert({identifiers[i], -1});
    }
}

void LocalBlockAtlas::registerBlock(LocalBlockDef def) {
    definitions.push_back(def);

    if (!identifierIndexTable.count(def.getIdentifier())) {
        std::cout << Log::err << "Client/Server block identifier desync: " + def.getIdentifier() + "!" << Log::endl;
        return;
    }
    identifierIndexTable[def.getIdentifier()] = def.getIndex();
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