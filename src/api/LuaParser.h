//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <sol.hpp>
#include <iostream>
#include "../util/Path.h"

class LuaParser {
public:
    explicit LuaParser(std::string root);
    std::string root;
    Path root_path;
protected:
    sol::state lua;
    sol::table zeus;

    int DoFileSandboxed(std::string file);
};


#endif //ZEUS_LUAPARSER_H
