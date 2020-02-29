//
// Created by aurailus on 2020-02-18.
//

#pragma once

#include <sol2/sol.hpp>

namespace Api {
    static void trigger_event(sol::state& lua) {
        lua.script(R"(
            zepha.__builtin.trigger_event = function(event, ...)
                if zepha.registered_callbacks[event] == nil then return nil end

                for _, v in pairs(zepha.registered_callbacks[event]) do
                    if (type(v) == "function") then
                        v(...)
                    end
                end
            end
        )");
    }
}