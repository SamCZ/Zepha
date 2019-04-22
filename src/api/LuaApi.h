//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <string>
#include <sol.hpp>
#include <iostream>

#include "func/LModuleRegister.h"
#include "func/LModuleUtil.h"

class GameDefs;

class LuaApi {
public:
    LuaApi() = default;

    void init(GameDefs& defs);

    void loadMod(std::string file);

    sol::state lua;
    sol::table zeus;

    int DoFileSandboxed(std::string file);

    ~LuaApi() = default;
private:
};


#endif //ZEUS_LUAPARSER_H