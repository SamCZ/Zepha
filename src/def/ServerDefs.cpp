//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

ServerDefs::ServerDefs() :
    luaApi("/home/aurailus/CLion/Zeus/res/lua/") {
}

void ServerDefs::init(ServerWorld &world) {
    luaApi.init(*this, world);
}

ServerBlockAtlas &ServerDefs::blocks() {
    return blockAtlas;
}

ServerLuaParser &ServerDefs::lua() {
    return luaApi;
}

void ServerDefs::update(double delta) {
    this->delta += delta;
    while (this->delta > 48) {
        luaApi.update();
        this->delta -= 48;
    }
}
