//
// Created by aurailus on 17/12/18.
//

#include "LuaApi.h"
#include "../def/LocalDefs.h"

//todo: add security using doFileSandboxed

void LuaApi::init(LocalDefs& defs) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);
    lua.script_file("/home/aurailus/C++/GlProject/res/lua/util.lua");

    zeus = lua.create_table();
    lua["zeus"] = zeus;

    lua.set_function("dofile", &LuaApi::DoFileSandboxed, this);

    LModuleRegister(lua, zeus, defs);
    LModuleUtil(lua, zeus, defs);
}

void LuaApi::loadMod(std::string file) {
    lua.script_file(file);
}

int LuaApi::DoFileSandboxed(std::string file) {
    std::cout << file << std::endl;
    lua.script_file(file);
}
