//
// Created by aurailus on 10/06/19.
//

#include "ServerDefs.h"

ServerDefs::ServerDefs() :
    lua("/home/aurailus/C++/GlProject/res/lua/") {

    blockAtlas = ServerBlockAtlas();

    lua.init(*this);
//    lua.loadMod("../res/lua/default/init.lua");
}

ServerBlockAtlas &ServerDefs::blocks() {
    return blockAtlas;
}
