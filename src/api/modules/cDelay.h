//
// Created by aurailus on 15/06/19.
//

#ifndef ZEUS_MODULECLIENTDELAY_H
#define ZEUS_MODULECLIENTDELAY_H

#include "../../def/LocalDefs.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void delay(sol::table &zeus, std::list<LocalLuaParser::DelayedFunction> &funcs) {
        zeus.set_function("delay", [&](sol::function function, float delay, sol::variadic_args args) {
            std::vector<sol::object> argsObject;
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back({function, argsObject, delay, delay});
        });
    }
}

#endif //ZEUS_MODULECLIENTDELAY_H
