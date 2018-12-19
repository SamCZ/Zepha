//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

//Should give tracebacks if there's a segfault
//#define SOL_CHECK_ARGUMENTS

#include <string>
#include <sol.hpp>

class LuaParser {
public:
    LuaParser() = default;

    void init();
    sol::table* getModule();
    void doFile(std::string file);

    ~LuaParser() = default;
private:
    sol::state L;
    sol::table Z;
};


#endif //ZEUS_LUAPARSER_H
