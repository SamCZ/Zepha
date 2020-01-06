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

             "set_pos", &LocalLuaEntity::set_pos,
             "int_pos", &LocalLuaEntity::int_pos,
             "get_pos", &LocalLuaEntity::get_pos,
             "set_visual_offset", &LocalLuaEntity::set_visual_offset,
             "int_visual_offset", &LocalLuaEntity::int_visual_offset,
             "get_visual_offset", &LocalLuaEntity::get_visual_offset,
             "set_yaw", &LocalLuaEntity::set_yaw,
             "int_yaw", &LocalLuaEntity::int_yaw,
             "get_yaw", &LocalLuaEntity::get_yaw,
             "set_scale", &LocalLuaEntity::set_scale,
             "int_scale", &LocalLuaEntity::int_scale,
             "get_scale", &LocalLuaEntity::get_scale,

             "pos", sol::property(&LocalLuaEntity::get_pos, &LocalLuaEntity::set_pos),
             "visual_offset", sol::property(&LocalLuaEntity::get_visual_offset, &LocalLuaEntity::set_visual_offset),
             "yaw", sol::property(&LocalLuaEntity::get_yaw, &LocalLuaEntity::set_yaw),
             "scale", sol::property(&LocalLuaEntity::get_scale, &LocalLuaEntity::set_scale),

             "set_display_type", &LocalLuaEntity::set_display_type
        );
    }
}