//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "../../../lua/LuaParser.h"

#include "../../hud/GuiBuilder.h"
#include "../../../lua/LuaMod.h"

class Subgame;
class AtlasRef;
class ClientState;
class GuiContainer;

class MenuSandbox : LuaParser {
public:
    MenuSandbox(glm::ivec2& win, ClientState& state, std::shared_ptr<GuiContainer> container);
    void load(const Subgame& subgame);

    void update(double delta) override;
    void windowResized();

    using LuaParser::update;
private:
    void reset();
    void loadApi();
    void loadAndRunMod(const std::string& modPath);

    void showError(const std::string& what, const std::string& subgame);

    sol::protected_function_result runFileSandboxed(const std::string& file);
    sol::protected_function_result errorCallback(sol::protected_function_result errPfr);

    LuaMod mod {};
    std::vector<std::shared_ptr<AtlasRef>> modAssets {};

    std::shared_ptr<GuiContainer> container = nullptr;
    std::shared_ptr<GuiContainer> luaContainer = nullptr;
    GuiBuilder builder;

    ClientState& state;
    glm::ivec2& win;
};
