//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

LocalDefs::LocalDefs(std::string tex_path) :
    lua("/home/aurailus/C++/GlProject/res/lua/") {

    textureAtlas = TextureAtlas(1024);
    textureAtlas.loadDirectory(tex_path + "/game");
    textureAtlas.loadDirectory(tex_path + "/gui");
    textureAtlas.loadDirectory(tex_path + "/ent");

    blockAtlas = LocalBlockAtlas();

    lua.init(*this);
}

LocalBlockAtlas &LocalDefs::blocks() {
    return blockAtlas;
}

TextureAtlas &LocalDefs::textures() {
    return textureAtlas;
}
