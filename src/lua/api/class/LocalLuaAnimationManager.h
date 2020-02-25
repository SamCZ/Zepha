//
// Created by aurailus on 2020-01-14.
//

#pragma once

#include "../../../game/entity/Entity.h"
#include <sol2/sol.hpp>

class LocalLuaAnimationManager {
public:
    LocalLuaAnimationManager(Entity& entity);

    void define(sol::table anims);
    LocalLuaAnimationManager& set_anim(sol::object anim, sol::optional<bool> loop);
    LocalLuaAnimationManager& play(sol::optional<unsigned int> offset);
    LocalLuaAnimationManager& pause(sol::optional<unsigned int> offset);
private:
    Entity& entity;
};
