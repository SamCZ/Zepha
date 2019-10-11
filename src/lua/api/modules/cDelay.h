//
// Created by aurailus on 15/06/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void delay(sol::table &core, std::list<LocalLuaParser::DelayedFunction> &funcs) {
        core.set_function("delay", [&](sol::function function, float delay, sol::variadic_args args) {
            std::vector<sol::object> argsObject;
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back(LocalLuaParser::DelayedFunction{function, argsObject, delay, delay});
        });
    }
}
