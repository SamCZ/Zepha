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
                std::map<std::string, std::function<void()>> callbackMap;
                for (auto &callback : *callbacks)  {
                    callbackMap.emplace(callback.first.as<std::string>(), [=]() {
                        callback.second.as<sol::function>()();
                    });
                }

                builder.setGui(gui, callbackMap);
            }
            else builder.setGui(gui);
            builder.build(win);
        });
    }
}
