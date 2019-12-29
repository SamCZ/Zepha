//
// Created by aurailus on 15/06/19.
//

#pragma once

#include "../../../def/ServerDefs.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void delay(sol::table &core, std::list<ServerLuaParser::DelayedFunction> &funcs) {
        core.set_function("delay", [&](sol::optional<sol::function> function, sol::optional<float> delay, sol::variadic_args args) {
            if (!function) throw "expected a function as the first argument.";
            if (!delay) throw "expected a number as the second argument.";

            std::vector<sol::object> argsObject {};
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back({*function, argsObject, *delay, *delay});
        });
    }
}
