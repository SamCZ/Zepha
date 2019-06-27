//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERDEFS_H
#define ZEUS_SERVERDEFS_H


#include "block/server/ServerBlockAtlas.h"
#include "../api/server/ServerLuaParser.h"

class ServerWorld;

class ServerDefs {
public:
    ServerDefs();

    ServerBlockAtlas& blocks();
    ServerLuaParser& lua();

    void init(ServerWorld& world);
    void update(double delta);

    ~ServerDefs() = default;
private:
    double delta = 0;

    ServerBlockAtlas blockAtlas;
    ServerLuaParser luaApi;
};


#endif //ZEUS_SERVERDEFS_H
