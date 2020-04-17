//
// Created by aurailus on 2019-10-19.
//

#pragma once

#include <sol2/sol.hpp>

#include "LocalLuaInventory.h"
#include "../../../def/ClientGame.h"
#include "../../../game/scene/world/Player.h"

class LocalLuaPlayer {
public:
    LocalLuaPlayer(Player& player) : player(player) {}

    Player& player;

    void set_pos(const sol::table& pos);
    sol::table get_pos(sol::this_state s);
    sol::table get_block_pos(sol::this_state s);

    void set_vel(const sol::table& vel);
    sol::table get_vel(sol::this_state s);

    void set_look_yaw(float rot);
    float get_look_yaw();

    void set_look_pitch(float rot);
    float get_look_pitch();

    void show_menu(std::shared_ptr<LuaGuiElement> root);
    void close_menu();
    bool is_in_menu();

    void set_hud(std::shared_ptr<LuaGuiElement> hud);
    std::shared_ptr<LuaGuiElement> get_hud();

    LocalLuaInventory get_inventory();

    void set_selected_block(std::string block);

    void set_flying(bool shouldFly);
    bool get_flying();
};

namespace ClientApi {
    static void local_player(sol::state& lua) {
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

            "get_inventory", &LocalLuaPlayer::get_inventory,
            "set_selected_block", &LocalLuaPlayer::set_selected_block,

            "show_menu", &LocalLuaPlayer::show_menu,
            "close_menu", &LocalLuaPlayer::close_menu,
            "set_hud", &LocalLuaPlayer::set_hud,
            "get_hud", &LocalLuaPlayer::get_hud,

            "pos", sol::property(&LocalLuaPlayer::get_pos, &LocalLuaPlayer::set_pos),
            "block_pos", sol::property(&LocalLuaPlayer::get_block_pos, &LocalLuaPlayer::set_pos),
            "vel", sol::property(&LocalLuaPlayer::get_vel, &LocalLuaPlayer::set_vel),
            "look_yaw", sol::property(&LocalLuaPlayer::get_look_yaw, &LocalLuaPlayer::set_look_yaw),
            "look_pitch", sol::property(&LocalLuaPlayer::get_look_pitch, &LocalLuaPlayer::set_look_pitch),

            "flying", sol::property(&LocalLuaPlayer::set_flying, &LocalLuaPlayer::get_flying),

            "in_menu", sol::property(&LocalLuaPlayer::is_in_menu)
        );
    }
}