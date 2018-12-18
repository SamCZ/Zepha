//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <string>

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

class LuaParser {
public:
    explicit LuaParser(std::string luaFileLoc);

    void regGlobalModule();
    void regFunc(const char *functionName, lua_CFunction any);

    ~LuaParser();
private:
    lua_State* L;
};


#endif //ZEUS_LUAPARSER_H
