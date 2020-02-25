//
// Created by aurailus on 2020-01-14.
//

#pragma once

#include "../class/LocalLuaAnimationManager.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void animation_manager(sol::state& lua) {
        lua.new_usertype<LocalLuaAnimationManager>("AnimationManager",
             "define", &LocalLuaAnimationManager::define,
             "set_anim", &LocalLuaAnimationManager::set_anim,
             "play", &LocalLuaAnimationManager::play,
             "pause", &LocalLuaAnimationManager::pause
        );
    }
}