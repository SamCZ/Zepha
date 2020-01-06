//
// Created by aurailus on 2020-01-05.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void update_entities(sol::state& lua) {
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