//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

//Note: This constructor is only for *uninitialized* LocalDef objects!
LocalDefs::LocalDefs(const LocalDefs &copy) : LocalDefs(copy.tex_path) {}

LocalDefs::LocalDefs(const std::string& tex_path) :
    luaApi("/home/aurailus/CLion/Zeus/res/lua/"),
    tex_path(tex_path),
    textureAtlas(1024) {

    textureAtlas.loadDirectory(tex_path + "/game");
    textureAtlas.loadDirectory(tex_path + "/gui");
    textureAtlas.loadDirectory(tex_path + "/ent");
}

void LocalDefs::initLuaApi(LocalWorld &world) {
    luaApi.init(*this, world);
}

LocalBlockAtlas &LocalDefs::blocks() {
    return blockAtlas;
}

TextureAtlas &LocalDefs::textures() {
    return textureAtlas;
}

LocalLuaParser &LocalDefs::lua() {
    return luaApi;
}

void LocalDefs::update(float delta) {
    this->delta += delta;
    while (this->delta > 48) {
        luaApi.update();
        this->delta -= 48;
    }
}