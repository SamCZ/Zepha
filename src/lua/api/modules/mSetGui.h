//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../game/hud/GuiBuilder.h"

namespace MenuApi {
    void set_gui(GuiBuilder& builder, glm::ivec2& win, sol::state& lua, sol::table& core) {
        core.set_function("set_gui", [&](std::string gui, sol::optional<sol::table> callbacks) {
            if (callbacks) {
                std::map<std::string, std::function<void(glm::ivec2)>> callbackMap;
                for (auto &callback : *callbacks)  {
                    callbackMap.emplace(callback.first.as<std::string>(), [=, &lua](glm::ivec2 pos) {
                        callback.second.as<sol::function>()(lua.create_table_with("x", pos.x, "y", pos.y));
                    });
                }

                builder.setGui(gui, callbackMap);
            }
            else builder.setGui(gui);
            builder.build(win);
        });
    }
}
