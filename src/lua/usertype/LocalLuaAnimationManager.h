//
// Created by aurailus on 2020-01-14.
//

#pragma once

#include "../Lua.h"

class DrawableEntity;

class LocalLuaAnimationManager {
public:
    LocalLuaAnimationManager(DrawableEntity& entity);

    void define(sol::table anims);
    LocalLuaAnimationManager& set_anim(sol::object anim, sol::optional<bool> loop);
    LocalLuaAnimationManager& play(sol::optional<unsigned int> offset);
    LocalLuaAnimationManager& pause(sol::optional<unsigned int> offset);
private:
    DrawableEntity& entity;
};