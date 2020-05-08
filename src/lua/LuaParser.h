//
// The LuaParser superclass for both the Client and Server Parsers.
// This contains some shared functions, structs, and variables used by both subclasses.
// Created by aurailus on 11/06/19.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>
#include <sol2/sol.hpp>

class LuaParser {
public:
    constexpr static double UPDATE_STEP {1 / 60.0};
    virtual void update(double delta);

    sol::table luaVec(glm::vec3 vec);
    static sol::table luaVec(sol::state_view s, glm::vec3 vec);

    sol::state lua;
    sol::table core;

    struct DelayedFunction {
        sol::function function;
        std::vector<sol::object> args;
        float timeout;
        float initial_timeout;
    };
    std::list<DelayedFunction> delayed_functions;
};

