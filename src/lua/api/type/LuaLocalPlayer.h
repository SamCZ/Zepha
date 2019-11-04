//
// Created by aurailus on 2019-10-19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/Player.h"

class LuaLocalPlayer {
public:
    LuaLocalPlayer(Player& player) : player(player) {}

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

    std::string get_menu_state();
    void open_menu(std::string menu);

    void set_selected_block(std::string block);
};
