//
// Created by aurailus on 15/06/19.
//

#ifndef ZEUS_MODULESERVERDELAY_H
#define ZEUS_MODULESERVERDELAY_H

#include "../../../def/ServerDefs.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void delay(sol::table &zeus, std::list<ServerLuaParser::DelayedFunction> &funcs) {
        zeus.set_function("delay", [&](sol::function function, float delay, sol::variadic_args args) {
            std::vector<sol::object> argsObject;
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back({function, argsObject, delay, delay});
        });
    }
}

#endif //ZEUS_MODULESERVERDELAY_H
