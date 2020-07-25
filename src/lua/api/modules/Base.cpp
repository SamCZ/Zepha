//
// Created by aurailus on 2020-07-24.
//

#include "Base.h"

#include "../../../def/ServerGame.h"
#include "../../../lua/LuaParser.h"

Api::Module::Base::Base(Api::Module::State &state, Game &game, sol::table &core) :
    state(state),
    game(game),
    core(core),
    lua(game.lua->lua) {}
