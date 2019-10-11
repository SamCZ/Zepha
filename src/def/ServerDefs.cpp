//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

void ServerDefs::init(ServerWorld &world, const std::string& path) {
    size_t exec = path.find_last_of('/');
    std::string folderPath = path.substr(0, exec + 1);
    luaApi.init(*this, world, folderPath);
}

ServerDefinitionAtlas &ServerDefs::defs() {
    return blockAtlas;
}

ServerLuaParser &ServerDefs::lua() {
    return luaApi;
}

AssetStorage &ServerDefs::assets() {
    return assetStorage;
}

void ServerDefs::update(double delta) {
    this->delta += delta;
    while (this->delta > 0.05f) {
        luaApi.update(this->delta);
        this->delta -= 0.05f;
    }
}
