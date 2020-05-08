//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "../LuaParser.h"

#include "LocalModHandler.h"
#include "../LuaKeybindHandler.h"

class ClientGame;
class ClientState;
class LocalWorld;
class Player;

class LocalLuaParser : public LuaParser {
public:
    LocalLuaParser();

    void init(ClientGame& defs, LocalWorld& world, Player& player, ClientState& state);
    void update(double delta);

    LocalModHandler& getHandler();

    template<typename... Args> void safe_function(sol::protected_function f, Args... args) const {
        auto res = f(args...);
        if (!res.valid()) errorCallback(res);
    }
private:
    void loadApi(ClientGame& defs, LocalWorld& world, Player& player);
    void registerDefs(ClientGame &defs);

    sol::protected_function_result errorCallback(sol::protected_function_result errPfr) const;
    sol::protected_function_result runFileSandboxed(std::string file);

    LuaKeybindHandler keybinds;
    LocalModHandler handler;
    double delta = 0;
};
