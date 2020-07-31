//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "LuaParser.h"

#include "LocalModHandler.h"
#include "LuaKeybindHandler.h"

class LocalPlayer;
class LocalWorld;
class ClientState;
class LocalSubgame;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(LocalSubgame& game);
    void init(LocalWorld& world, ClientState& state);
    void loadPlayer(std::shared_ptr<LocalPlayer> player);

    void update(double delta);

    LocalModHandler& getHandler();
private:
    void loadApi(LocalSubgame &defs, LocalWorld &world);
    void registerDefs(LocalSubgame &defs);

    virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const override;
    sol::protected_function_result runFileSandboxed(const std::string& file);

    LocalSubgame& game;

    LuaKeybindHandler keybinds;
    LocalModHandler handler;
    double delta = 0;
};
