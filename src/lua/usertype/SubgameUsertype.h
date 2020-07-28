//
// Created by aurailus on 2020-07-27.
//

#pragma once

#include "BaseUsertype.h"

class State;

namespace Api {
    namespace Usertype {
        class SubgameUsertype : public BaseUsertype {
        public:
            SubgameUsertype() = default;
            static void bind(State state, sol::state& lua, sol::table& core) {};
        };
    }
}