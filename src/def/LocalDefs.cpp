//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

LocalDefs::LocalDefs(std::string tex_path) {
    textureAtlas = TextureAtlas(1024);
    textureAtlas.loadDirectory(tex_path + "/game");
    textureAtlas.loadDirectory(tex_path + "/gui");
    textureAtlas.loadDirectory(tex_path + "/ent");

    blockAtlas = LocalBlockAtlas();

    luaApi.init(*this);
    luaApi.loadMod("../res/lua/default/init.lua");
}

LocalBlockAtlas &LocalDefs::blocks() {
    return blockAtlas;
}

TextureAtlas &LocalDefs::textures() {
    return textureAtlas;
}
