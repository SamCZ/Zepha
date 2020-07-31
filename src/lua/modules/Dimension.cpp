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
    std::shared_ptr<::Dimension> dim = state == State::CLIENT ?
        std::static_pointer_cast<::Dimension>(static_cast<LocalWorld&>(world).getDefaultDimensionPtr()) :
        std::static_pointer_cast<::Dimension>(static_cast<ServerWorld&>(world).getDefaultDimensionPtr());
    return sol::make_object(s, Api::Usertype::Dimension(dim));
}

sol::object Api::Module::Dimension::getDimension(sol::this_state s, const std::string& identifier) {
    std::shared_ptr<::Dimension> dim = state == State::CLIENT ?
        std::static_pointer_cast<::Dimension>(static_cast<LocalWorld&>(world).getDimensionPtr(identifier)) :
        std::static_pointer_cast<::Dimension>(static_cast<ServerWorld&>(world).getDimensionPtr(identifier));
    return sol::make_object(s, Api::Usertype::Dimension(dim));
}