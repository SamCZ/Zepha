//
// Created by aurailus on 03/10/19.
//

#pragma once

#include "../Lua.h"
#include "../../util/CovariantPtr.h"
#include "LocalLuaAnimationManager.h"
#include "../../game/entity/DrawableEntity.h"

class LocalSubgame;

class LocalLuaEntity {
public:
    LocalLuaEntity(std::unique_ptr<DrawableEntity> entity, unsigned int id, SubgamePtr game) :
        entity(std::move(entity)), id(id), game(game), manager(*this->entity) {}

    std::unique_ptr<DrawableEntity> entity = nullptr;
    unsigned int id;
    SubgamePtr game;

    LocalLuaAnimationManager manager;

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

    void set_display_type(const std::string& type, const std::string& arg, sol::optional<std::string> arg2);
};