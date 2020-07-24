//
// Created by aurailus on 11/10/19.
//

#pragma once

#include "../../Lua.h"

namespace Api {
    static void update_entities(sol::state& lua) {
        lua.script(R"(
            zepha.__builtin.update_entities = function(delta)
                for k, v in pairs(zepha.entities) do
                    if (type(v.on_update) == "function") then
                        v:on_update(delta)
                    end
                end
            end
        )");
    }
}