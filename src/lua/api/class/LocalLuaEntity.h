//
// Created by aurailus on 03/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../game/entity/Entity.h"
#include "../../../def/ClientGame.h"
#include "LocalLuaAnimationManager.h"

class LocalLuaEntity {
public:
    LocalLuaEntity(std::unique_ptr<Entity> entity, unsigned int id, ClientGame& defs) :
        entity(std::move(entity)), id(id), defs(defs), manager(*this->entity) {}

    std::unique_ptr<Entity> entity = nullptr;
    unsigned int id;
    ClientGame& defs;

    LocalLuaAnimationManager manager;

    void snap_pos(const sol::table& pos);
    void set_pos(const sol::table& pos);
    sol::table get_pos(sol::this_state s);

    void snap_visual_offset(const sol::table& vs);
    void set_visual_offset(const sol::table& pos);
    sol::table get_visual_offset(sol::this_state s);

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