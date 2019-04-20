//
// Created by aurailus on 02/12/18.
//

#include "BlockAtlas.h"

BlockAtlas::BlockAtlas() {
    textureAtlas = nullptr;
}

BlockAtlas::BlockAtlas(TextureAtlas *textureAtlas) {
    this->textureAtlas = textureAtlas;
}

void BlockAtlas::registerBlock(BlockDef def) {
    definitions.push_back(def);
}

BlockDef& BlockAtlas::getBlock(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at((unsigned long)id);
    }

    //TODO: Remove hardcoded reference to air
    std::cerr << "Block ID #" << id << " (undefined) requested! Returnin air." << std::endl;
    return definitions.at(0);
}