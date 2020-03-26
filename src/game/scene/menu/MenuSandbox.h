//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <fstream>
#include <cute_files/cute_files.h>
#include "Subgame.h"
#include "../../ClientState.h"
#include "../../hud/GuiBuilder.h"
#include "../../hud/components/basic/GuiContainer.h"
#include "../../../lua/LuaMod.h"
#include "../../../lua/LuaParser.h"


class MenuSandbox : LuaParser {
public:
    MenuSandbox(glm::ivec2& win, ClientState& state, std::shared_ptr<GuiContainer> container);
    void load(const Subgame& subgame);

    void windowResized();

    using LuaParser::update;
private:
    void reset();
    void loadApi();
    void executeMods();
    void loadAndRunMod(const std::string& modPath);

    sol::protected_function_result runFileSandboxed(const std::string& file);
    sol::protected_function_result errorCallback(sol::protected_function_result errPfr);

    LuaMod mod {};
    std::vector<std::shared_ptr<AtlasRef>> modAssets {};

    std::shared_ptr<GuiContainer> container = nullptr;

    ClientState& state;
    GuiBuilder builder;
    glm::ivec2& win;
};
