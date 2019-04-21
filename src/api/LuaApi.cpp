//
// Created by aurailus on 17/12/18.
//

#include "LuaApi.h"
#include "../def/GameDefs.h"

void LuaApi::init(GameDefs& defs) {
    L.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);

    Z = L.create_table();
    L["zeus"] = Z;

    LModuleRegister(L, Z, defs);
}

sol::table* LuaApi::getModule() {
    return &Z;
}

sol::state* LuaApi::getState() {
    return &L;
}

void LuaApi::doFile(std::string file) {
    L.script_file(file);
}