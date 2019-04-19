//
// Created by aurailus on 18/04/19.
//

#include "GameDefs.h"

GameDefs::GameDefs(std::string tex_path) {
    textures = TextureAtlas(512);
    textures.loadFromDirectory(std::move(tex_path));

    blocks = BlockAtlas(&textures);
}

BlockAtlas &GameDefs::getBlockAtlas() {
    return blocks;
}

TextureAtlas &GameDefs::getTextureAtlas() {
    return textures;
}
