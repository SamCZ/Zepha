//
// The LuaParser superclass for both the Client and Server Parsers.
// This contains some shared functions, structs, and variables used by both subclasses.
// Created by aurailus on 11/06/19.
//

#pragma once

#include <list>
#include <glm/vec3.hpp>

#include "modules/SubgameModule.h"

#include "Lua.h"
#include "../util/CovariantPtr.h"

class Subgame;

class LuaParser {
public:
    constexpr static double UPDATE_STEP {1 / 60.};

    LuaParser(Subgame& game) : game(game) {};
    virtual void update(double delta) = 0;
    
    void bindModules();

    template<typename... Args> sol::protected_function_result safe_function(sol::protected_function f, Args... args) const {
        auto res = f(args...);
        if (!res.valid()) errorCallback(res);
        return res;
    }

    virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const = 0;

    Subgame& game;

    sol::state lua;
    sol::table core;
    
    std::vector<std::unique_ptr<Api::Module::BaseModule>> modules;
};