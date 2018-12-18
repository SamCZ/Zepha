//
// Created by aurailus on 17/12/18.
//

#include <iostream>
#include "LuaParser.h"
#include "apis/l_Average.h"

LuaParser::LuaParser(std::string luaFileLoc) {
    L = luaL_newstate();
    luaL_openlibs(L);

    regGlobalModule();

    int err = luaL_dofile(L, luaFileLoc.c_str());
    if (err != 0) {
        std::cerr << "Failed to compile " << luaFileLoc << " with error code " << err << "." << std::endl;
        std::cerr << lua_tostring(L, -1) << std::endl;
        luaL_traceback(L, L, nullptr, 1);
        std::cerr << lua_tostring(L, -1) << std::endl;
    }

    getchar(); // Just to pause
}

void LuaParser::regFunc(const char *functionName, lua_CFunction any) {
    lua_pushcfunction(L, any);
    lua_setfield(L, -2, functionName);
}


void LuaParser::regGlobalModule() {
    lua_newtable(L);

    l_Average().bind_methods(this);

    lua_setglobal(L, "zeus");
}

LuaParser::~LuaParser() {
    lua_close(L);
}