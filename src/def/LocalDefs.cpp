//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

//Note: This constructor is only for *uninitialized* LocalDef objects!
LocalDefs::LocalDefs(const LocalDefs &copy) : LocalDefs(copy.tex_path) {}

LocalDefs::LocalDefs(const std::string& path) :
    luaApi(),
    tex_path(path),
    textureAtlas(8192) {

    textureAtlas.loadDirectory(tex_path);
}

void LocalDefs::initLuaApi(LocalWorld &world, GameGui& gui) {
    luaApi.init(*this, world, gui);
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