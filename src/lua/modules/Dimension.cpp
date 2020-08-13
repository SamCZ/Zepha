//
// Created by aurailus on 2020-07-30.
//

#include "Dimension.h"

#include "../Lua.h"
#include "../usertype/Dimension.h"
#include "world/LocalWorld.h"
#include "world/ServerWorld.h"

void Api::Module::Dimension::bind() {
    core.set_function("create_dimension", Util::bind_this(this, &Dimension::createDimension));
    core.set_function("get_dimension", Util::bind_this(this, &Dimension::getDimension));
    core.set_function("get_default_dimension", Util::bind_this(this, &Dimension::getDefaultDimension));
    core.set_function("set_default_dimension", Util::bind_this(this, &Dimension::setDefaultDimension));
}

Api::Usertype::Dimension Api::Module::Dimension::createDimension(const std::string& identifier) {
    return Api::Usertype::Dimension(world.createDimension(identifier));
}

void Api::Module::Dimension::setDefaultDimension(const std::string& identifier) {
    world.setDefaultDimension(identifier);
}

sol::object Api::Module::Dimension::getDefaultDimension(const sol::this_state s) {
    auto dim = world.getDefaultDimension();
    if (!dim) return sol::nil;
    return sol::make_object(s, Api::Usertype::Dimension(dim));
}

sol::object Api::Module::Dimension::getDimension(sol::this_state s, const std::string& identifier) {
    auto dim = world.getDimension(identifier);
    if (!dim) return sol::nil;
    return sol::make_object(s, Api::Usertype::Dimension(dim));
}