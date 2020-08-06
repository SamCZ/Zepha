//
// Created by aurailus on 2020-01-14.
//

#include "AnimationManager.h"

#include "../Lua.h"
#include "../../game/entity/LuaEntity.h"
#include "../../game/entity/DrawableEntity.h"
#include "../../game/entity/AnimationSegment.h"

Api::Usertype::AnimationManager::AnimationManager(EntityPtr entity) :
    entity(entity) {}

Api::Usertype::AnimationManager& Api::Usertype::AnimationManager::define(sol::table anims) {
    std::vector<AnimationSegment> animations;

    for (auto& ref : anims) {
        std::string n = ref.first.as<std::string>();
        sol::table t = ref.second.as<sol::table>();
        animations.push_back({n, {static_cast<unsigned int>(t.get<float>(1)), static_cast<unsigned int>(t.get<float>(2))}});
    }

    entity->animation.setAnimations(animations);

    return *this;
}

Api::Usertype::AnimationManager& Api::Usertype::AnimationManager::set_anim(sol::object anim, sol::optional<bool> loop) {
    if (anim.is<std::string>()) entity->animation.setAnim(anim.as<std::string>(), 0, loop.value_or(true));
    else if (anim.is<sol::table>()) entity->animation.setAnim(
        glm::ivec2 {anim.as<sol::table>().get<unsigned int>(1), anim.as<sol::table>().get<unsigned int>(2)}, 0, loop.value_or(true));

    return *this;
}

Api::Usertype::AnimationManager& Api::Usertype::AnimationManager::play(sol::optional<unsigned int> offset) {
    entity->animation.setPlaying(true);
    if (offset) entity->animation.setFrame(entity->animation.getFrame() + *offset);
    return *this;
}

Api::Usertype::AnimationManager& Api::Usertype::AnimationManager::pause(sol::optional<unsigned int> offset) {
    entity->animation.setPlaying(false);
    if (offset) entity->animation.setFrame(entity->animation.getFrame() + *offset);
    return *this;
}

void Api::Usertype::AnimationManager::bind(State, sol::state &lua, sol::table &core) {
    lua.new_usertype<AnimationManager>("AnimationManager",
        "define", &AnimationManager::define,
        "set_anim", &AnimationManager::set_anim,
        "play", &AnimationManager::play,
        "pause", &AnimationManager::pause
    );
}
