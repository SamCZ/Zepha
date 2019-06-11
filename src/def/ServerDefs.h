//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERDEFS_H
#define ZEUS_SERVERDEFS_H

#include "block/server/ServerBlockAtlas.h"
#include "../api/LuaApi.h"

class ServerDefs {
public:
    ServerDefs();

    ServerBlockAtlas& blocks();

    ~ServerDefs() = default;
private:
    ServerBlockAtlas blockAtlas;
//    LuaApi luaApi;
};


#endif //ZEUS_SERVERDEFS_H
