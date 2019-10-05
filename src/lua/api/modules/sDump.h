//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void dump(sol::state& lua) {
        lua.script(R"(
            function dump(tbl, indent)
                if not indent then indent = 0 end
                for k, v in pairs(tbl) do
                    if type(k) == "number" then
                        k = "[" .. k .. "]"
                    end
                    if type(v) == "function" then
                        v = "[function]"
                    end
                    local indentString = string.rep("  ", indent)
                    local formatting = indentString .. k .. " = "
                    if type(v) == "table" then
                        print(formatting .. "{")
                        dump(v, indent+1)
                        print(indentString .. "}")
                    elseif type(v) == 'boolean' then
                        print(formatting .. tostring(v))
                    else
                        print(formatting .. v)
                    end
                end
            end
        )");
    }
}
