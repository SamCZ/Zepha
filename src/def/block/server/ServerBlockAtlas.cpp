//
// Created by aurailus on 10/06/19.
//

#include "ServerBlockAtlas.h"

ServerBlockAtlas::ServerBlockAtlas() {
    //Register Air Node
    ServerBlockModel nullModel; nullModel.visible = false, nullModel.culls = false;
    ServerBlockDef air("builtin:air", nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    registerBlock(std::move(air));
}

void ServerBlockAtlas::registerBlock(ServerBlockDef def) {
    definitions.push_back(def);
}

ServerBlockDef& ServerBlockAtlas::fromIndex(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at((unsigned long)id);
    }

    std::cerr << "Block ID #" << id << " (undefined) requested! Returning air." << std::endl;
    return definitions.at(0);
}

ServerBlockDef &ServerBlockAtlas::fromIdentifier(std::string identifier) {
    if (identifierIndexTable.count(identifier) > 0) {
        return definitions.at(static_cast<unsigned long>(identifierIndexTable.at(identifier)));
    }

    std::cerr << "Block Identifier \"" << identifier << "\" (undefined) requested! Returning air." << std::endl;
    return definitions.at(0);
}
