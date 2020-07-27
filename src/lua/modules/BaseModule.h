//
// Created by aurailus on 2020-07-26.
//

#pragma once

#include "sol/forward.hpp"

namespace Api {
    enum class State { CLIENT, SERVER };

    namespace Module {
        class BaseModule {
        public:
            BaseModule(State state, sol::state& lua, sol::table& core);
            virtual void bind() = 0;

        protected:
            State state;

            sol::state& lua;
            sol::table& core;
        };
    }
}