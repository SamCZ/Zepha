//
// Created by aurailus on 2020-07-30.
//

#include "Dimension.h"

#include "../Lua.h"
#include "../usertype/Dimension.h"
#include "../../game/scene/world/LocalWorld.h"
#include "../../net/server/world/ServerWorld.h"

void Api::Module::Dimension::bind() {
    core.set_function("create_dimension", Util::bind_this(this, &Dimension::createDimension));
    core.set_function("get_dimension", Util::bind_this(this, &Dimension::getDimension));
    core.set_function("get_default_dimension", Util::bind_this(this, &Dimension::getDefaultDimension));
    core.set_function("set_default_dimension", Util::bind_this(this, &Dimension::setDefaultDimension));
}

void Api::Module::Dimension::createDimension(const std::string& identifier) {
    world.createDimension(identifier);
}

void Api::Module::Dimension::setDefaultDimension(const std::string& identifier) {
    world.setDefaultDimension(identifier);
}

sol::object Api::Module::Dimension::getDefaultDimension(const sol::this_state s) {
    return sol::make_object(s, Api::Usertype::Dimension(world.getDefaultDimension()));
}

sol::object Api::Module::Dimension::getDimension(sol::this_state s, const std::string& identifier) {
    return sol::make_object(s, Api::Usertype::Dimension(world.getDimension(identifier)));
}