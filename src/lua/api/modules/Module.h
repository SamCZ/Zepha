//
// Created by aurailus on 2020-07-23.
//

#pragma once

#include <functional>

#include "sol/forward.hpp"

class World;
class Subgame;

namespace Api {
    enum class State { CLIENT, SERVER };

    namespace Module {
        class Module {
        public:
            Module(State state, Subgame& game, World& world, sol::table& core);
            virtual void bind() = 0;

        protected:
            State state;

            World& world;
            Subgame& game;

            sol::state& lua;
            sol::table& core;
        };
    }
}