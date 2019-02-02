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

void BlockAtlas::registerBlock(BlockDef* def) {
    definitions.push_back(def);
}

BlockDef* BlockAtlas::getBlock(int id) {
    if (id >= 0 && id < definitions.size()) {
        return definitions.at((unsigned long)id);
    }
    std::cout << "Block ID " << id << " is not defined! Returning null pointer." << std::endl;
    return nullptr;
}

BlockAtlas::~BlockAtlas() {
    for (auto b : definitions) {
        delete b;
    }
    definitions.clear();
}