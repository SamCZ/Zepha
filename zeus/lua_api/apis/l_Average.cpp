//
// Created by aurailus on 17/12/18.
//

#include "l_Average.h"

int average(lua_State *L) {
    int n = lua_gettop(L);
    double sum = 0;

    for (int i = 1; i <= n; i++) {
        sum += lua_tonumber(L, i);
    }

    lua_pushnumber(L, sum / n);
    return 1;
}

void l_Average::bind_methods(LuaParser* parser) {
    parser->regFunc("average", &average);
}