//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../lua/LuaParser.h"

namespace Api {
    static void delay(sol::table& core, std::list<LuaParser::DelayedFunction>& funcs) {
        core.set_function("delay", [&](sol::optional<sol::function> function, sol::optional<float> delay, sol::variadic_args args) {
            if (!function) throw "expected a function as the first argument.";
            if (!delay) throw "expected a number as the second argument.";

            std::vector<sol::object> argsObject {};
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back({*function, argsObject, *delay, *delay});
        });
    }
}
