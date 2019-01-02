//
// Created by aurailus on 01/01/19.
//

#ifndef ZEUS_LUAAPI_H
#define ZEUS_LUAAPI_H


#include "../game/GameInstance.h"
#include "LuaParser.h"

class LuaApi {
public:
    LuaApi(GameInstance* game, LuaParser* parser);

    GameInstance* game;
    LuaParser* parser;
};


#endif //ZEUS_LUAAPI_H
