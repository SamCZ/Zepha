//
// Created by aurailus on 2020-07-23.
//

#pragma once

#include "BaseModule.h"

class World;
class Subgame;

namespace Api {
    namespace Module {
        class SubgameModule : public BaseModule {
        public:
            SubgameModule(State state, sol::table& core, Subgame& game, World& world);
            virtual void bind() = 0;

        protected:
            State state;

            World& world;
            Subgame& game;
        };
    }
}