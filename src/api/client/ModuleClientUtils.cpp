//
// Created by aurailus on 21/04/19.
//

#include "ModuleClientUtils.h"

ModuleClientUtils::ModuleClientUtils(sol::state &lua, sol::table &zeus, LocalDefs &defs) {

    //Dump function
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

    //Get Path for mod
    zeus.set_function("get_path", [&](std::string modname) {
        try {
            if (modname.length() == 0) throw "No mod name specified!";
            return "/home/aurailus/C++/GlProject/res/lua/" + modname + "/";
        }
        catch (const std::string& e) {
            std::cerr << "Exception on get_path: " << e << std::endl;
        }
    });

    //Get if the interpreter is the server or the client
    zeus.set_function("is_server", []() {
        return false;
    });
}
