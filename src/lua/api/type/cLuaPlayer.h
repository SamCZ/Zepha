//
// Created by aurailus on 2019-10-19.
//

#pragma once

#include "LuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "LuaPlayer.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void player(sol::state& lua, LocalWorld& world) {
        lua.new_usertype<LuaPlayer>("LuaPlayer",
                "set_pos", &LuaPlayer::set_pos,
                "get_pos", &LuaPlayer::get_pos,
                "get_block_pos", &LuaPlayer::get_block_pos,
                "set_vel", &LuaPlayer::set_vel,
                "get_vel", &LuaPlayer::get_vel,
                "set_look_yaw", &LuaPlayer::set_look_yaw,
                "get_look_yaw", &LuaPlayer::get_look_yaw,
                "set_look_pitch", &LuaPlayer::set_look_pitch,
                "get_look_pitch", &LuaPlayer::get_look_pitch,

                "pos", sol::property(&LuaPlayer::get_pos, &LuaPlayer::set_pos),
                "block_pos", sol::property(&LuaPlayer::get_block_pos, &LuaPlayer::set_pos),
                "vel", sol::property(&LuaPlayer::get_vel, &LuaPlayer::set_vel),
                "look_yaw", sol::property(&LuaPlayer::get_look_yaw, &LuaPlayer::set_look_yaw),
                "look_yaw", sol::property(&LuaPlayer::get_look_pitch, &LuaPlayer::set_look_pitch),

                "menu_state", sol::property(&LuaPlayer::get_menu_state),
                "set_selected_block", &LuaPlayer::set_selected_block
        );
    }
}