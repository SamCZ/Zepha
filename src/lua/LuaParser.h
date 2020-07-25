//
// The LuaParser superclass for both the Client and Server Parsers.
// This contains some shared functions, structs, and variables used by both subclasses.
// Created by aurailus on 11/06/19.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>

#include "Lua.h"

class LuaParser {
public:
    constexpr static double UPDATE_STEP {1. / 60.};
    virtual void update(double delta) = 0;

    sol::state lua;
    sol::table core;
};

