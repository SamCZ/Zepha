//
// Created by aurailus on 17/12/18.
//

#include "LuaParser.h"

void LuaParser::init() {
    L.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

    Z = L.create_table();
    L["zeus"] = Z;
}

sol::table* LuaParser::getModule() {
    return &Z;
}

sol::state* LuaParser::getState() {
    return &L;
}

void LuaParser::doFile(std::string file) {
    L.script_file(file);
}