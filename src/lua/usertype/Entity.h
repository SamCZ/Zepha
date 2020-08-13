//
// Created by aurailus on 2020-08-04.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameUsertype.h"

#include "Dimension.h"
#include "AnimationManager.h"
#include "../../util/CovariantPtr.h"
#include "../../game/entity/LocalLuaEntity.h"

namespace Api::Usertype {
    class Entity : public SubgameUsertype {
    public:
        Entity(EntityPtr entity) : entity(entity), animation(entity.isL() ?
             std::static_pointer_cast<AnimationManager>(std::make_shared<LocalAnimationManager>(entity)) :
             std::static_pointer_cast<AnimationManager>(std::make_shared<ServerAnimationManager>(entity))) {}

        EntityPtr entity;
        std::shared_ptr<AnimationManager> animation;

        sol::object get_animation_manager(sol::this_state s);

        unsigned int get_id();

        void snap_pos(glm::vec3 pos);
        void set_pos(glm::vec3 pos);
        glm::vec3 get_pos();

        void snap_visual_offset(glm::vec3 vs);
        void set_visual_offset(glm::vec3 vs);
        glm::vec3 get_visual_offset();

        void snap_pitch(float rot);
        void set_pitch(float rot);
        float get_pitch();

        void snap_yaw(float rot);
        void set_yaw(float rot);
        float get_yaw();

        void snap_roll(float rot);
        void set_roll(float rot);
        float get_roll();

        void snap_scale(float scale);
        void set_scale(float scale);
        float get_scale();

        Dimension get_dimension();
        void set_dimension(const std::string& identifier);

        void set_display_type(const std::string& type, const std::string& arg, sol::optional<std::string> arg2);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}