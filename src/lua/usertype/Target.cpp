//
// Created by aurailus on 2020-07-27.
//

#include "Target.h"

#include "../Lua.h"

Api::Usertype::Target::Target(const ::Target &target) :
    pos(target.pos),
    type(target.type),
    dim(Dimension(target.dim)),
    pos_above(target.getAbovePos()) {}

std::string Api::Usertype::Target::getType() {
    return type == ::Target::Type::BLOCK ? "block" :
           type == ::Target::Type::ENTITY ? "entity" :
           "nothing";
}

void Api::Usertype::Target::bind(State, sol::state &lua, sol::table &core) {
    lua.new_usertype<Target>("Target",
        "type", sol::property(&Target::getType),

        "pos", sol::readonly(&Target::pos),
        "dim", sol::readonly(&Target::dim),
        "pos_above", sol::readonly(&Target::pos_above)
    );
}