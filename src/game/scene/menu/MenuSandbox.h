//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <fstream>
#include <cute_files/cute_files.h>
#include "../../ClientState.h"
#include "../../../lua/LuaParser.h"
#include "../../../lua/LuaMod.h"
#include "../../hud/components/basic/GUIContainer.h"
#include "../../hud/GuiBuilder.h"


class MenuSandbox : LuaParser {
public:
    MenuSandbox(glm::ivec2& win, ClientState& state, std::shared_ptr<GUIContainer> container);
    void load(const std::string& subgamePath);

    void windowResized();

    using LuaParser::update;
private:
    void setup();
    void loadMod(const std::string& modPath);

    sol::protected_function_result DoFileSandboxed(std::string file);
    LuaMod mod {};

    std::shared_ptr<GUIContainer> container = nullptr;
    ClientState& state;
    GuiBuilder builder;
    glm::ivec2& win;
};
