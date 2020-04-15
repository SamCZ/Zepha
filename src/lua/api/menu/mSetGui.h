//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <sol2/sol.hpp>
#include "../class/LuaGuiElement.h"
#include "../../../game/hud/GuiBuilder.h"

namespace MenuApi {
    void set_gui(GuiBuilder& builder, glm::ivec2& win, sol::state& lua, sol::table& core) {
        core.set_function("set_gui", [&](std::shared_ptr<LuaGuiElement> gui) {
            builder.setGuiRoot(gui);
            builder.build(win);
        });
    }
}
