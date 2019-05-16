//
// Created by aurailus on 18/04/19.
//

#include "GameDefs.h"

GameDefs::GameDefs(std::string tex_path) {
    textureAtlas = TextureAtlas(1024);
    textureAtlas.loadDirectory(tex_path + "/game");
    textureAtlas.loadDirectory(tex_path + "/gui");
    textureAtlas.loadDirectory(tex_path + "/ent");

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
