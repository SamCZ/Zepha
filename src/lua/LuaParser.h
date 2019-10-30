//
// Created by aurailus on 11/06/19.
// The Lua API superclass for both the client and the server.
// This class contains platform agnostic functions and structures for both of the APIs.
//
//

#pragma once

#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include <sol2/sol.hpp>

#include "../util/Log.h"

class LuaParser {
public:
    static inline void override_panic(sol::optional<std::string> message) {
        std::cout << Log::err << "Zepha has panicked! Error:" << Log::endl;
        if (message) std::cout << Log::err << message.value() << Log::endl;
    }

    struct DelayedFunction {
        sol::function function;
        std::vector<sol::object> args;
        float timeout;
        float initial_timeout;
    };

    virtual void update(double delta);

    sol::table vecToTable(glm::vec3 vec);

    sol::state lua;
    sol::table core;

    std::list<DelayedFunction> delayed_functions;
};

