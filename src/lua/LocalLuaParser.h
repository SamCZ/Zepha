//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "LuaParser.h"

#include "LocalModHandler.h"
#include "LuaKeybindHandler.h"
#include "../util/CovariantPtr.h"

class LocalPlayer;
class LocalWorld;
class ClientState;
class LocalSubgame;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(LocalSubgame& game);
    void init(WorldPtr world, ClientState& state);
    void loadPlayer(PlayerPtr player);

    void update(double delta) override;

    LocalModHandler& getHandler();
private:
    void loadApi(WorldPtr world);
    void registerDefs();

    virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const override;
    sol::protected_function_result runFileSandboxed(const std::string& file);

    LuaKeybindHandler keybinds;
    LocalModHandler handler;
    double delta = 0;
};
