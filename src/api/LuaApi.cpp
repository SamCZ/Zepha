//
// Created by aurailus on 17/12/18.
//

#include "LuaApi.h"
#include "../def/GameDefs.h"

void LuaApi::init(GameDefs& defs) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    zeus = lua.create_table();
    lua["zeus"] = zeus;

    lua.set_function("dofile", &LuaApi::DoFileSandboxed, this);

    LModuleRegister(lua, zeus, defs);
    LModuleUtil(lua, zeus, defs);
}

sol::table* LuaApi::getModule() {
    return &zeus;
}

sol::state* LuaApi::getState() {
    return &lua;
}

void LuaApi::doFile(std::string file) {
    lua.script_file(file);
}

int LuaApi::DoFileSandboxed(std::string file) {
    std::cout << file << std::endl;
    lua.script_file(file);
}
