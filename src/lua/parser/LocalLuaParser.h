//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "../LuaParser.h"

#include "LocalModHandler.h"
#include "../LuaKeybindHandler.h"

class Player;
class LocalWorld;
class ClientState;
class LocalSubgame;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(LocalSubgame& game);
    void init(LocalWorld& world, Player& player, ClientState& state);

    void update(double delta);

    LocalModHandler& getHandler();

    template<typename... Args> sol::safe_function_result safe_function(sol::protected_function f, Args... args) const {
        auto res = f(args...);
        if (!res.valid()) errorCallback(res);
        return res;
    }
private:
    void loadApi(LocalSubgame& defs, LocalWorld& world, Player& player);
    void registerDefs(LocalSubgame &defs);

    sol::protected_function_result errorCallback(sol::protected_function_result errPfr) const;
    sol::protected_function_result runFileSandboxed(const std::string& file);

    LocalSubgame& game;

    LuaKeybindHandler keybinds;
    LocalModHandler handler;
    double delta = 0;
};
