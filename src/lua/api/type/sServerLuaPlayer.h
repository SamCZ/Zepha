//
// Created by aurailus on 2020-02-17.
//

#pragma once

#include <sol2/sol.hpp>
#include "ServerLuaPlayer.h"

namespace ServerApi {
    void server_player(sol::state& lua) {
        lua.new_usertype<ServerLuaPlayer>("ServerPlayer",
              "is_player", sol::readonly(&ServerLuaPlayer::is_player),

              "set_pos", &ServerLuaPlayer::set_pos,
              "get_pos", &ServerLuaPlayer::get_pos,
              "get_block_pos", &ServerLuaPlayer::get_block_pos,
//              "set_vel", &ServerLuaPlayer::set_vel,
//              "get_vel", &ServerLuaPlayer::get_vel,
              "set_look_yaw", &ServerLuaPlayer::set_look_yaw,
              "get_look_yaw", &ServerLuaPlayer::get_look_yaw,
              "set_look_pitch", &ServerLuaPlayer::set_look_pitch,
              "get_look_pitch", &ServerLuaPlayer::get_look_pitch,

              "get_inventory", &ServerLuaPlayer::get_inventory,

//              "open_menu", &ServerLuaPlayer::open_menu,
//              "close_menu", &ServerLuaPlayer::close_menu,

              "pos", sol::property(&ServerLuaPlayer::get_pos, &ServerLuaPlayer::set_pos),
              "block_pos", sol::property(&ServerLuaPlayer::get_block_pos, &ServerLuaPlayer::set_pos),
//              "vel", sol::property(&ServerLuaPlayer::get_vel, &ServerLuaPlayer::set_vel),
              "look_yaw", sol::property(&ServerLuaPlayer::get_look_yaw, &ServerLuaPlayer::set_look_yaw),
              "look_yaw", sol::property(&ServerLuaPlayer::get_look_pitch, &ServerLuaPlayer::set_look_pitch),

              "flying", sol::property(&ServerLuaPlayer::set_flying, &ServerLuaPlayer::get_flying),

              "name", sol::property(&ServerLuaPlayer::get_name),
              "cid", sol::property(&ServerLuaPlayer::get_cid),
              "address", sol::property(&ServerLuaPlayer::get_address)

//              "menu_state", sol::property(&ServerLuaPlayer::get_menu_state)
        );
    }
}