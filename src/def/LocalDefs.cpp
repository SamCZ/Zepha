//
// Created by aurailus on 18/04/19.
//

#include "LocalDefs.h"

//Note: This constructor is only for *uninitialized* LocalDef objects!
LocalDefs::LocalDefs(const LocalDefs &copy) : LocalDefs(copy.tex_path) {}

LocalDefs::LocalDefs(const std::string& path) :
    tex_path(path),

    luaApi(),
    biomes(),
    textures(2048),
    defs(textures) {

    textures.loadDirectory(tex_path);
}

void LocalDefs::init(LocalWorld &world, Player& player) {
    luaApi.init(*this, world, player);
}

void LocalDefs::update(double delta, bool* keys) {
    luaApi.update(delta, keys);
}
