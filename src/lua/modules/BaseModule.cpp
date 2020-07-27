//
// Created by aurailus on 2020-07-26.
//

#include "BaseModule.h"

Api::Module::BaseModule::BaseModule(Api::State state, sol::state& lua, sol::table& core) :
    state(state), lua(lua), core(core) {}
