//
// Created by aurailus on 2020-02-17.
//

#pragma once

#include "../Lua.h"
#include "ServerLuaInventory.h"
#include "../../net/server/conn/ServerClient.h"

class ServerLuaPlayer {
public:
    ServerLuaPlayer(ServerClient& player) : player(player), cid(player.cid) {}

    ServerClient& player;
    bool is_player;

    std::string get_name();
    unsigned int get_cid();
    std::string get_address();

    glm::vec3 get_pos();
    glm::vec3 get_block_pos();
    void set_pos(glm::vec3 pos);

    glm::vec3 get_vel();
    void set_vel(glm::vec3 vel);

    float get_look_yaw();
    void set_look_yaw(float rot);

    float get_look_pitch();
    void set_look_pitch(float rot);

    ServerLuaInventory get_inventory();

    sol::object get_hand_list(sol::this_state s);
    void set_hand_list(sol::object handList);
    sol::object get_hand_stack(sol::this_state s);

    sol::object get_wield_list(sol::this_state s);
    sol::object get_wield_stack(sol::this_state s);
    void set_wield_list(sol::object list);

    unsigned int get_wield_index();
    void set_wield_index(unsigned int index);

    void set_flying(bool shouldFly);
    bool get_flying();

private:
    unsigned int cid;
};

namespace ServerApi {
    static void server_player(sol::state& lua) {
        lua.new_usertype<ServerLuaPlayer>("ServerPlayer",
            "is_player", sol::readonly(&ServerLuaPlayer::is_player),

            "get_pos", &ServerLuaPlayer::get_pos,
            "get_block_pos", &ServerLuaPlayer::get_block_pos,
            "set_pos", &ServerLuaPlayer::set_pos,
            "get_vel", &ServerLuaPlayer::get_vel,
            "set_vel", &ServerLuaPlayer::set_vel,
            "get_look_yaw", &ServerLuaPlayer::get_look_yaw,
            "set_look_yaw", &ServerLuaPlayer::set_look_yaw,
            "get_look_pitch", &ServerLuaPlayer::get_look_pitch,
            "set_look_pitch", &ServerLuaPlayer::set_look_pitch,

            "get_inventory", &ServerLuaPlayer::get_inventory,
            "get_hand_list", &ServerLuaPlayer::get_hand_list,
            "set_hand_list", &ServerLuaPlayer::set_hand_list,
            "get_hand_stack", &ServerLuaPlayer::get_hand_stack,
            "get_wield_list", &ServerLuaPlayer::get_wield_list,
            "set_wield_list", &ServerLuaPlayer::set_wield_list,
            "get_wield_index", &ServerLuaPlayer::get_wield_index,
            "set_wield_index", &ServerLuaPlayer::set_wield_index,
            "get_wield_stack", &ServerLuaPlayer::get_wield_stack,

//            "open_menu", &ServerLuaPlayer::open_menu,
//            "close_menu", &ServerLuaPlayer::close_menu,

            "pos", sol::property(&ServerLuaPlayer::get_pos, &ServerLuaPlayer::set_pos),
            "block_pos", sol::property(&ServerLuaPlayer::get_block_pos, &ServerLuaPlayer::set_pos),
            "vel", sol::property(&ServerLuaPlayer::get_vel, &ServerLuaPlayer::set_vel),
            "look_yaw", sol::property(&ServerLuaPlayer::get_look_yaw, &ServerLuaPlayer::set_look_yaw),
            "look_pitch", sol::property(&ServerLuaPlayer::get_look_pitch, &ServerLuaPlayer::set_look_pitch),

            "flying", sol::property(&ServerLuaPlayer::set_flying, &ServerLuaPlayer::get_flying),

            "name", sol::property(&ServerLuaPlayer::get_name),
            "cid", sol::property(&ServerLuaPlayer::get_cid),
            "address", sol::property(&ServerLuaPlayer::get_address)
        );
    }
}