//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <string>
#include <sol.hpp>
#include <iostream>

#include "func/LModuleRegister.h"

class GameDefs;

class LuaApi {
public:
    LuaApi() = default;

    void init(GameDefs& defs);

    sol::table* getModule();
    sol::state* getState();

    void doFile(std::string file);

    sol::state L;
    sol::table Z;

    ~LuaApi() = default;
private:
};


#endif //ZEUS_LUAPARSER_H