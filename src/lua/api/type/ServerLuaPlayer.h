//
// Created by aurailus on 2020-02-17.
//

#pragma once

#include <sol2/sol.hpp>
#include "LuaInventory.h"
#include "../../../server/conn/ServerClient.h"

class ServerLuaPlayer {
public:
    ServerLuaPlayer(ServerClient& player) : player(player), cid(player.cid) {}

    ServerClient& player;
    unsigned int cid;
    bool is_player = true;

    void set_pos(const sol::table& pos);
    sol::table get_pos(sol::this_state s);
    sol::table get_block_pos(sol::this_state s);

//    void set_vel(const sol::table& vel);
//    sol::table get_vel(sol::this_state s);

    void set_look_yaw(float rot);
    float get_look_yaw();

    void set_look_pitch(float rot);
    float get_look_pitch();

//    std::string get_menu_state();
//    void close_menu();
//    void open_menu(sol::this_state s, std::string menu, sol::optional<sol::table> callbacks);

    LuaInventory get_inventory();

//    void set_selected_block(std::string block);

    void set_flying(bool shouldFly);
    bool get_flying();

    std::string get_name();
    unsigned int get_cid();
    std::string get_address();
};
