//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "LocalModHandler.h"
#include "../LuaMod.h"
#include "../LuaParser.h"
#include "../LuaInputManager.h"

class ClientGame;
class LocalWorld;
class Player;

class LocalLuaParser : public LuaParser {
public:
    void init(ClientGame& defs, LocalWorld& world, Player& player);
    void update(double delta, bool* keys);

    LocalModHandler& getHandler();
private:
    void loadApi(ClientGame& defs, LocalWorld& world, Player& player);
    void registerDefs(ClientGame &defs);

    sol::protected_function_result errorCallback(lua_State*, sol::protected_function_result errPfr);
    sol::protected_function_result runFileSandboxed(std::string file);

    LuaInputManager manager;
    LocalModHandler handler;
    double delta = 0;
};
