//
// Created by aurailus on 30/08/19.
//

#pragma once

#include "../../../game/entity/world/LuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void entity(sol::state& lua, LocalWorld& world) {
        lua.new_usertype<LuaEntity>("LuaEntity",
                "set_pos", &LuaEntity::set_pos,
                "int_pos", &LuaEntity::int_pos,
                "get_pos", &LuaEntity::get_pos,
                "set_visual_offset", &LuaEntity::set_visual_offset,
                "int_visual_offset", &LuaEntity::int_visual_offset,
                "get_visual_offset", &LuaEntity::get_visual_offset,
                "set_yaw", &LuaEntity::set_yaw,
                "int_yaw", &LuaEntity::int_yaw,
                "get_yaw", &LuaEntity::get_yaw,
                "set_scale", &LuaEntity::set_scale,
                "int_scale", &LuaEntity::int_scale,
                "get_scale", &LuaEntity::get_scale,

                "pos", sol::property(&LuaEntity::get_pos, &LuaEntity::set_pos),
                "visual_offset", sol::property(&LuaEntity::get_visual_offset, &LuaEntity::set_visual_offset),
                "yaw", sol::property(&LuaEntity::get_yaw, &LuaEntity::set_yaw),
                "scale", sol::property(&LuaEntity::get_scale, &LuaEntity::set_scale),

                "set_display_type", &LuaEntity::set_display_type
        );
    }
}