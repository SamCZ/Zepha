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
                std::map<std::string, GuiBuilder::ComponentCallbacks> callbackMap;

                for (auto& pair : *callbacks)  {
                    if (!pair.first.is<std::string>() || !pair.second.is<sol::table>()) continue;
                    std::string identifier = pair.first.as<std::string>();
                    sol::table callbacks = pair.second.as<sol::table>();
                    GuiBuilder::ComponentCallbacks componentCallbacks {};

                    auto left  = callbacks.get<sol::optional<sol::function>>("left");
                    auto right = callbacks.get<sol::optional<sol::function>>("right");
                    auto hover = callbacks.get<sol::optional<sol::function>>("hover");

                    if (left) componentCallbacks.left = [=, &lua](bool down, glm::ivec2 pos) {
                            (*left)(down, lua.create_table_with("x", pos.x, "y", pos.y)); };

                    if (right) componentCallbacks.right = [=, &lua](bool down, glm::ivec2 pos) {
                            (*right)(down, lua.create_table_with("x", pos.x, "y", pos.y)); };

                    if (hover) componentCallbacks.hover = [=, &lua](bool down, glm::ivec2 pos) {
                            (*hover)(down, lua.create_table_with("x", pos.x, "y", pos.y)); };

                    callbackMap.emplace(identifier, componentCallbacks);
                }

                builder.setGui(gui, callbackMap);
            }
            else builder.setGui(gui);
            builder.build(win);
        });
    }
}
