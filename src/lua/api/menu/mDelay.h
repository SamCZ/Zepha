//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <list>
#include <sol2/sol.hpp>

#include "../../LuaParser.h"

namespace MenuApi {
    void delay(sol::table &core, std::list<LuaParser::DelayedFunction> &funcs) {
        core.set_function("delay", [&](sol::function function, float delay, sol::variadic_args args) {
            std::vector<sol::object> argsObject {};
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back(LuaParser::DelayedFunction{function, argsObject, delay, delay});
        });
    }
}
