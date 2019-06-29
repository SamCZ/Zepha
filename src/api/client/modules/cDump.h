//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_CDUMP_H
#define ZEUS_CDUMP_H

#include <sol.hpp>

namespace ClientApi {
    void dump(sol::state& lua) {
        lua.script(R"(
            local function dump(tbl, indent)
                if not indent then indent = 0 end
                for k, v in pairs(tbl) do
                    if type(k) == "number" then
                        k = "[" .. k .. "]"
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

            zeus.dump = dump
        )");
    }
}

#endif //ZEUS_CDUMP_H
