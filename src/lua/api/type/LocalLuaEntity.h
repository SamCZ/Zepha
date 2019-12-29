//
// Created by aurailus on 03/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../game/entity/Entity.h"
#include "../../../def/LocalDefs.h"

class LocalLuaEntity {
public:
    LocalLuaEntity(std::unique_ptr<Entity> entity, unsigned int id, LocalDefs& defs) :
        entity(std::move(entity)), id(id), defs(defs) {}

    std::unique_ptr<Entity> entity = nullptr;
    unsigned int id;
    LocalDefs& defs;

    void set_pos(const sol::table& pos);
    void int_pos(const sol::table& pos);
    sol::table get_pos(sol::this_state s);

    void set_visual_offset(const sol::table& vs);
    void int_visual_offset(const sol::table& pos);
    sol::table get_visual_offset(sol::this_state s);

    void set_yaw(float rot);
    void int_yaw(float rot);
    float get_yaw();

    void set_scale(float scale);
    void int_scale(float scale);
    float get_scale();

    void set_display_type(const std::string& type, const std::string& arg, sol::optional<std::string> arg2);
};