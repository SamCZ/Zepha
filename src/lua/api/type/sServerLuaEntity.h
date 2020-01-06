//
// Created by aurailus on 2020-01-05.
//

#pragma once

#include "ServerLuaEntity.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void entity(sol::state& lua) {
        lua.new_usertype<ServerLuaEntity>("LuaEntity",
             "id", sol::readonly(&ServerLuaEntity::id),

             "set_pos", &ServerLuaEntity::set_pos,
             "int_pos", &ServerLuaEntity::int_pos,
             "get_pos", &ServerLuaEntity::get_pos,
             "set_visual_offset", &ServerLuaEntity::set_visual_offset,
             "int_visual_offset", &ServerLuaEntity::int_visual_offset,
             "get_visual_offset", &ServerLuaEntity::get_visual_offset,
             "set_yaw", &ServerLuaEntity::set_yaw,
             "int_yaw", &ServerLuaEntity::int_yaw,
             "get_yaw", &ServerLuaEntity::get_yaw,
             "set_scale", &ServerLuaEntity::set_scale,
             "int_scale", &ServerLuaEntity::int_scale,
             "get_scale", &ServerLuaEntity::get_scale,

             "pos", sol::property(&ServerLuaEntity::get_pos, &ServerLuaEntity::set_pos),
             "visual_offset", sol::property(&ServerLuaEntity::get_visual_offset, &ServerLuaEntity::set_visual_offset),
             "yaw", sol::property(&ServerLuaEntity::get_yaw, &ServerLuaEntity::set_yaw),
             "scale", sol::property(&ServerLuaEntity::get_scale, &ServerLuaEntity::set_scale),

             "set_display_type", &ServerLuaEntity::set_display_type
        );
    }
}