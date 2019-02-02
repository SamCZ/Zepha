//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <string>
#include <sol.hpp>
#include <iostream>

class LuaParser {
public:
    LuaParser() = default;

    void init();

    sol::table* getModule();
    sol::state* getState();

    void doFile(std::string file);

    ~LuaParser() = default;
private:
    sol::state L;
    sol::table Z;
};


#endif //ZEUS_LUAPARSER_H