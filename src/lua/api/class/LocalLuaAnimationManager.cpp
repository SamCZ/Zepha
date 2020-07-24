//
// Created by aurailus on 2020-01-14.
//

#include "LocalLuaAnimationManager.h"

#include "../../Lua.h"
#include "../../../game/entity/Entity.h"
#include "../../../game/entity/AnimationSegment.h"

LocalLuaAnimationManager::LocalLuaAnimationManager(Entity &entity) :
    entity(entity) {}

void LocalLuaAnimationManager::define(sol::table anims) {
    std::vector<AnimationSegment> animations;
    for (auto& ref : anims) {
        std::string n = ref.first.as<std::string>();
        sol::table t = ref.second.as<sol::table>();
        animations.push_back({n, static_cast<unsigned int>(t.get<float>(1)), static_cast<unsigned int>(t.get<float>(2))});
    }

    entity.setAnimations(animations);
}

LocalLuaAnimationManager& LocalLuaAnimationManager::set_anim(sol::object anim, sol::optional<bool> loop) {
    if (anim.is<std::string>()) {
        entity.playAnimation(anim.as<std::string>(), loop.value_or(true));
    }
    else if (anim.is<sol::table>()) {
        entity.playRange(anim.as<sol::table>().get<unsigned int>(1), anim.as<sol::table>().get<unsigned int>(2), loop.value_or(true));
    }

    return *this;
}

LocalLuaAnimationManager& LocalLuaAnimationManager::play(sol::optional<unsigned int> offset) {
    entity.setPlaying(true, offset.value_or(UINT_MAX));
    return *this;
}

LocalLuaAnimationManager& LocalLuaAnimationManager::pause(sol::optional<unsigned int> offset) {
    entity.setPlaying(false, offset.value_or(UINT_MAX));
    return *this;
}
