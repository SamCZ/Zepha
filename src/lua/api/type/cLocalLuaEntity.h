//
// Created by aurailus on 30/08/19.
//

#pragma once

#include "LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void entity(sol::state& lua, LocalWorld& world) {
        lua.new_usertype<LocalLuaEntity>("LuaEntity",
             "id", sol::readonly(&LocalLuaEntity::id),

             "snap_pos", &LocalLuaEntity::snap_pos,
             "set_pos", &LocalLuaEntity::set_pos,
             "get_pos", &LocalLuaEntity::get_pos,
             "snap_visual_offset", &LocalLuaEntity::snap_visual_offset,
             "set_visual_offset", &LocalLuaEntity::set_visual_offset,
             "get_visual_offset", &LocalLuaEntity::get_visual_offset,
             "snap_yaw", &LocalLuaEntity::snap_yaw,
             "set_yaw", &LocalLuaEntity::set_yaw,
             "get_yaw", &LocalLuaEntity::get_yaw,
             "snap_scale", &LocalLuaEntity::snap_scale,
             "set_scale", &LocalLuaEntity::set_scale,
             "get_scale", &LocalLuaEntity::get_scale,

             "pos", sol::property(&LocalLuaEntity::get_pos, &LocalLuaEntity::set_pos),
             "visual_offset", sol::property(&LocalLuaEntity::get_visual_offset, &LocalLuaEntity::set_visual_offset),
             "yaw", sol::property(&LocalLuaEntity::get_yaw, &LocalLuaEntity::set_yaw),
             "scale", sol::property(&LocalLuaEntity::get_scale, &LocalLuaEntity::set_scale),

             "set_display_type", &LocalLuaEntity::set_display_type
        );
    }
}