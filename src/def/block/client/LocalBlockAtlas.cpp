//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockAtlas.h"

LocalBlockAtlas::LocalBlockAtlas() {
    //Register Air Node
    LocalBlockModel nullModel; nullModel.visible = false, nullModel.culls = false;
    LocalBlockDef air("builtin:air", static_cast<int>(definitions.size()), nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    registerBlock(std::move(air));
}

int LocalBlockAtlas::definitionsSize() {
    return static_cast<int>(definitions.size());
}

void LocalBlockAtlas::registerBlock(LocalBlockDef def) {
    definitions.push_back(def);
    identifierIndexTable.insert({def.getIdentifier(), def.getIndex()});
}

LocalBlockDef& LocalBlockAtlas::fromIndex(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at(static_cast<unsigned long>(id));
    }

    std::cerr << "Block ID #" << id << " (undefined) requested! Returning air." << std::endl;
    return definitions.at(0);
}

LocalBlockDef &LocalBlockAtlas::fromIdentifier(std::string identifier) {
    if (identifierIndexTable.count(identifier) > 0) {
        return definitions.at(static_cast<unsigned long>(identifierIndexTable.at(identifier)));
    }

    std::cerr << "Block Identifier \"" << identifier << "\" (undefined) requested! Returning air." << std::endl;
    return definitions.at(0);
}
