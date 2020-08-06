//
// Created by aurailus on 2020-01-14.
//

#pragma once

#include "../Lua.h"

#include "../../util/CovariantPtr.h"
#include "SubgameUsertype.h"

namespace Api::Usertype {
    class AnimationManager : public SubgameUsertype  {
    public:
        AnimationManager(EntityPtr entity);

        EntityPtr entity;

        AnimationManager& define(sol::table anims);
        AnimationManager& set_anim(sol::object anim, sol::optional<bool> loop);
        AnimationManager& play(sol::optional<unsigned int> offset);
        AnimationManager& pause(sol::optional<unsigned int> offset);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}