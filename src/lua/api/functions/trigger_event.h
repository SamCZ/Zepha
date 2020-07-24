//
// Created by aurailus on 2020-02-18.
//

#pragma once

#include "../../Lua.h"

namespace Api {
    static void trigger_event(sol::state& lua) {
        lua.script(R"(
            zepha.__builtin.trigger_event = function(event, ...)
                if zepha.registered_callbacks[event] == nil then return nil end

                for _, EVENT_CALLBACK in pairs(zepha.registered_callbacks[event]) do
                    if (type(EVENT_CALLBACK) == "function") then
                        EVENT_CALLBACK(...)
                    end
                end
            end
        )");
    }
}