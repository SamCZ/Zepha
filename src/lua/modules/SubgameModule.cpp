//
// Created by aurailus on 2020-07-24.
//

#include "SubgameModule.h"

#include "game/Subgame.h"
#include "lua/LuaParser.h"

Api::Module::SubgameModule::SubgameModule(State state, sol::table& core, Subgame& game, World& world) :
    BaseModule(state, game.getParser().lua, core), game(game), world(world) {}