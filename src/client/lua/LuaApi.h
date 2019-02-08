//
// Created by aurailus on 01/01/19.
//

#ifndef ZEUS_LUAAPI_H
#define ZEUS_LUAAPI_H

#include "LuaParser.h"
#include "../game/GameScene.h"

class LuaApi {
public:
    LuaApi(GameScene* game, LuaParser* parser);

    GameScene* game;
    LuaParser* parser;
};


#endif //ZEUS_LUAAPI_H
