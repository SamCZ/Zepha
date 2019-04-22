//
// Created by aurailus on 02/12/18.
//

#include "BlockAtlas.h"

BlockAtlas::BlockAtlas() {
    //Register Air Node
    BlockModel nullModel;
    nullModel.visible = false;
    nullModel.culls = false;
    BlockDef air("builtin:air", nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    registerBlock(std::move(air));
}

void BlockAtlas::registerBlock(BlockDef def) {
    definitions.push_back(def);
}

BlockDef& BlockAtlas::getBlock(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at((unsigned long)id);
    }

    std::cerr << "Block ID #" << id << " (undefined) requested! Returning air." << std::endl;
    return definitions.at(0);
}