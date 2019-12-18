//
// Created by aurailus on 2019-10-19.
//

#pragma once

#include "LuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "LuaLocalPlayer.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void local_player(sol::state& lua, LocalWorld& world) {
        lua.new_usertype<LuaLocalPlayer>("LocalPlayer",
             "set_pos", &LuaLocalPlayer::set_pos,
             "get_pos", &LuaLocalPlayer::get_pos,
             "get_block_pos", &LuaLocalPlayer::get_block_pos,
             "set_vel", &LuaLocalPlayer::set_vel,
             "get_vel", &LuaLocalPlayer::get_vel,
             "set_look_yaw", &LuaLocalPlayer::set_look_yaw,
             "get_look_yaw", &LuaLocalPlayer::get_look_yaw,
             "set_look_pitch", &LuaLocalPlayer::set_look_pitch,
             "get_look_pitch", &LuaLocalPlayer::get_look_pitch,

             "pos", sol::property(&LuaLocalPlayer::get_pos, &LuaLocalPlayer::set_pos),
             "block_pos", sol::property(&LuaLocalPlayer::get_block_pos, &LuaLocalPlayer::set_pos),
             "vel", sol::property(&LuaLocalPlayer::get_vel, &LuaLocalPlayer::set_vel),
             "look_yaw", sol::property(&LuaLocalPlayer::get_look_yaw, &LuaLocalPlayer::set_look_yaw),
             "look_yaw", sol::property(&LuaLocalPlayer::get_look_pitch, &LuaLocalPlayer::set_look_pitch),

             "menu_state", sol::property(&LuaLocalPlayer::get_menu_state),
             "open_menu", &LuaLocalPlayer::open_menu,
             "close_menu", &LuaLocalPlayer::close_menu,

             "get_inventory", &LuaLocalPlayer::get_inventory,
             "set_selected_block", &LuaLocalPlayer::set_selected_block
        );
    }
}