//
// Created by aurailus on 30/08/19.
//

#pragma once

#include "../class/LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void entity(sol::state& lua) {
        lua.new_usertype<LocalLuaEntity>("LuaEntity",
             "id", sol::readonly(&LocalLuaEntity::id),

             "snap_pos", &LocalLuaEntity::snap_pos,
             "set_pos", &LocalLuaEntity::set_pos,
             "get_pos", &LocalLuaEntity::get_pos,
             "snap_visual_offset", &LocalLuaEntity::snap_visual_offset,
             "set_visual_offset", &LocalLuaEntity::set_visual_offset,
             "get_visual_offset", &LocalLuaEntity::get_visual_offset,
             "snap_pitch", &LocalLuaEntity::snap_pitch,
             "set_pitch", &LocalLuaEntity::set_pitch,
             "get_pitch", &LocalLuaEntity::get_pitch,
             "snap_yaw", &LocalLuaEntity::snap_yaw,
             "set_yaw", &LocalLuaEntity::set_yaw,
             "get_yaw", &LocalLuaEntity::get_yaw,
             "snap_roll", &LocalLuaEntity::snap_roll,
             "set_roll", &LocalLuaEntity::set_roll,
             "get_roll", &LocalLuaEntity::get_roll,
             "snap_scale", &LocalLuaEntity::snap_scale,
             "set_scale", &LocalLuaEntity::set_scale,
             "get_scale", &LocalLuaEntity::get_scale,

             "pos", sol::property(&LocalLuaEntity::get_pos, &LocalLuaEntity::set_pos),
             "visual_offset", sol::property(&LocalLuaEntity::get_visual_offset, &LocalLuaEntity::set_visual_offset),
             "pitch", sol::property(&LocalLuaEntity::get_pitch, &LocalLuaEntity::set_pitch),
             "yaw", sol::property(&LocalLuaEntity::get_yaw, &LocalLuaEntity::set_yaw),
             "roll", sol::property(&LocalLuaEntity::get_roll, &LocalLuaEntity::set_roll),
             "scale", sol::property(&LocalLuaEntity::get_scale, &LocalLuaEntity::set_scale),

             "anims", sol::readonly(&LocalLuaEntity::manager),

             "set_display_type", &LocalLuaEntity::set_display_type
        );
    }
}