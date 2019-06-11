//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

ServerDefs::ServerDefs() {

    blockAtlas = ServerBlockAtlas();

//    luaApi.init(*this);
//    luaApi.loadMod("../res/lua/default/init.lua");
}

ServerBlockAtlas &ServerDefs::blocks() {
    return blockAtlas;
}
