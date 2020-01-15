//
// Created by aurailus on 2019-10-19.
//

#pragma once

#include "LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "LocalLuaPlayer.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void local_player(sol::state& lua) {
        lua.new_usertype<LocalLuaPlayer>("LocalPlayer",
            "set_pos", &LocalLuaPlayer::set_pos,
            "get_pos", &LocalLuaPlayer::get_pos,
            "get_block_pos", &LocalLuaPlayer::get_block_pos,
            "set_vel", &LocalLuaPlayer::set_vel,
            "get_vel", &LocalLuaPlayer::get_vel,
            "set_look_yaw", &LocalLuaPlayer::set_look_yaw,
            "get_look_yaw", &LocalLuaPlayer::get_look_yaw,
            "set_look_pitch", &LocalLuaPlayer::set_look_pitch,
            "get_look_pitch", &LocalLuaPlayer::get_look_pitch,

            "pos", sol::property(&LocalLuaPlayer::get_pos, &LocalLuaPlayer::set_pos),
            "block_pos", sol::property(&LocalLuaPlayer::get_block_pos, &LocalLuaPlayer::set_pos),
            "vel", sol::property(&LocalLuaPlayer::get_vel, &LocalLuaPlayer::set_vel),
            "look_yaw", sol::property(&LocalLuaPlayer::get_look_yaw, &LocalLuaPlayer::set_look_yaw),
            "look_yaw", sol::property(&LocalLuaPlayer::get_look_pitch, &LocalLuaPlayer::set_look_pitch),

            "menu_state", sol::property(&LocalLuaPlayer::get_menu_state),
            "open_menu", &LocalLuaPlayer::open_menu,
            "close_menu", &LocalLuaPlayer::close_menu,

            "get_inventory", &LocalLuaPlayer::get_inventory,
            "set_selected_block", &LocalLuaPlayer::set_selected_block
        );
    }
}