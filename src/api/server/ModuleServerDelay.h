//
// Created by aurailus on 13/06/19.
//

#ifndef ZEUS_MODULESERVERDELAY_H
#define ZEUS_MODULESERVERDELAY_H

#include "../../def/ServerDefs.h"
#include <sol.hpp>

class ModuleServerDelay {

public:
    ModuleServerDelay(sol::state& lua, sol::table& zeus, ServerDefs& defs, std::list<ServerLuaParser::DelayedFunction>& funcs) {
        zeus.set_function("delay", [&](sol::function function, float delay, sol::variadic_args args) {
            std::vector<sol::object> argsObject;
            for (auto arg : args) argsObject.push_back(arg);
            funcs.push_back({function, argsObject, delay, delay});
        });
    }
};

#endif //ZEUS_MODULESERVERDELAY_H
