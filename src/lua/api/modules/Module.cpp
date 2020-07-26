//
// Created by aurailus on 2020-07-24.
//

#include "Module.h"

#include "../../../def/Subgame.h"
#include "../../../lua/LuaParser.h"

Api::Module::Module::Module(State state, Subgame &game, World& world, sol::table &core) :
    state(state),
    world(world),
    game(game),
    core(core),
    lua(game.getParser().lua) {}
