//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

void ServerDefs::init(ServerWorld &world, const std::string& path) {
    size_t exec = path.find_last_of('/');
    std::string folderPath = path.substr(0, exec + 1);
    luaApi.init(*this, world, folderPath);
}

ServerBlockAtlas &ServerDefs::blocks() {
    return blockAtlas;
}

ServerLuaParser &ServerDefs::lua() {
    return luaApi;
}

ServerTextureStorage &ServerDefs::textures() {
    return textureStorage;
}

void ServerDefs::update(double delta) {
    this->delta += delta;
    while (this->delta > 48) {
        luaApi.update();
        this->delta -= 48;
    }
}
