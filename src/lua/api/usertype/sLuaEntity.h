//
// Created by aurailus on 2020-01-05.
//

#pragma once

#include "../class/ServerLuaEntity.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void entity(sol::state& lua) {
        lua.new_usertype<ServerLuaEntity>("LuaEntity",
             "id", sol::readonly(&ServerLuaEntity::id),

             "snap_pos", &ServerLuaEntity::snap_pos,
             "set_pos", &ServerLuaEntity::set_pos,
             "get_pos", &ServerLuaEntity::get_pos,
             "snap_visual_offset", &ServerLuaEntity::snap_visual_offset,
             "set_visual_offset", &ServerLuaEntity::set_visual_offset,
             "get_visual_offset", &ServerLuaEntity::get_visual_offset,
             "snap_pitch", &ServerLuaEntity::snap_pitch,
             "set_pitch", &ServerLuaEntity::set_pitch,
             "get_pitch", &ServerLuaEntity::get_pitch,
             "snap_yaw", &ServerLuaEntity::snap_yaw,
             "set_yaw", &ServerLuaEntity::set_yaw,
             "get_yaw", &ServerLuaEntity::get_yaw,
             "snap_roll", &ServerLuaEntity::snap_roll,
             "set_roll", &ServerLuaEntity::set_roll,
             "get_roll", &ServerLuaEntity::get_roll,
             "snap_scale", &ServerLuaEntity::snap_scale,
             "set_scale", &ServerLuaEntity::set_scale,
             "get_scale", &ServerLuaEntity::get_scale,

             "pos", sol::property(&ServerLuaEntity::get_pos, &ServerLuaEntity::set_pos),
             "visual_offset", sol::property(&ServerLuaEntity::get_visual_offset, &ServerLuaEntity::set_visual_offset),
             "pitch", sol::property(&ServerLuaEntity::get_pitch, &ServerLuaEntity::set_pitch),
             "yaw", sol::property(&ServerLuaEntity::get_yaw, &ServerLuaEntity::set_yaw),
             "roll", sol::property(&ServerLuaEntity::get_roll, &ServerLuaEntity::set_roll),
             "scale", sol::property(&ServerLuaEntity::get_scale, &ServerLuaEntity::set_scale),

             "set_display_type", &ServerLuaEntity::set_display_type
        );
    }
}