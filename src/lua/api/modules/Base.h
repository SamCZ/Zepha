//
// Created by aurailus on 2020-07-23.
//

#pragma once

#include <functional>

#include "sol/forward.hpp"

class Game;

namespace Api {
    namespace Module {
        enum class State { CLIENT, SERVER };

        class Base {
        public:
            Base(State& state, Game& game, sol::table& core);
            virtual void bind() = 0;

        protected:
            Game& game;
            State& state;

            sol::state& lua;
            sol::table& core;
        };
    }
}