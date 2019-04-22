//
// Created by aurailus on 18/04/19.
//

#include "GameDefs.h"

GameDefs::GameDefs(std::string tex_path) {
    textureAtlas = TextureAtlas(128);
    textureAtlas.loadFromDirectory(std::move(tex_path));
//    textureAtlas.loadFromDirectory("../res/tex/gui");

    blockAtlas = BlockAtlas();

    luaApi.init(*this);
    luaApi.loadMod("../res/lua/default/init.lua");
}

BlockAtlas &GameDefs::blocks() {
    return blockAtlas;
}

TextureAtlas &GameDefs::textures() {
    return textureAtlas;
}
