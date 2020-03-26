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

    bool is_in_menu();
    void close_menu();
    void show_menu(sol::this_state s, sol::table menu);

    LocalLuaInventory get_inventory();

    void set_selected_block(std::string block);

    void set_flying(bool shouldFly);
    bool get_flying();
};
