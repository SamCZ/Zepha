//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

//Note: This constructor is only for *uninitialized* LocalDef objects!
LocalDefs::LocalDefs(const LocalDefs &copy) : LocalDefs(copy.tex_path) {}

LocalDefs::LocalDefs(const std::string& path) :
    luaApi(),
    tex_path(path),
    textureAtlas(2048) {

    textureAtlas.loadDirectory(tex_path);
}

void LocalDefs::initLuaApi(LocalWorld &world, GameGui& gui) {
    luaApi.init(*this, world, gui);
}

LocalDefinitionAtlas &LocalDefs::defs() {
    return blockAtlas;
}

TextureAtlas &LocalDefs::textures() {
    return textureAtlas;
}

ModelStore &LocalDefs::models() {
    return modelStore;
}

LocalLuaParser &LocalDefs::lua() {
    return luaApi;
}

void LocalDefs::update(double delta) {
    this->delta += delta;
    while (this->delta > 0.05f) {
        luaApi.update(this->delta);
        this->delta -= 0.05f;
    }
}