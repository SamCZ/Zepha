//
// Created by aurailus on 2019-10-19.
//

#pragma once

//#include <sol/forward.hpp>

#include "LocalLuaInventory.h"

class LuaGuiElement;
class Player;

class LocalLuaPlayer {
public:
    LocalLuaPlayer(Player& player) : player(player) {}

    Player& player;

    glm::vec3 get_pos();
    glm::vec3 get_block_pos();
    void set_pos(glm::vec3 pos);

    glm::vec3 get_vel();
    void set_vel(glm::vec3 vel);

    float get_look_yaw();
    void set_look_yaw(float rot);

    float get_look_pitch();
    void set_look_pitch(float rot);

    bool is_in_menu();
    void show_menu(std::shared_ptr<LuaGuiElement> root);
    void close_menu();

    std::shared_ptr<LuaGuiElement> get_hud();
    void set_hud(std::shared_ptr<LuaGuiElement> hud);

    LocalLuaInventory get_inventory();

    sol::object get_hand_list(sol::this_state s);
    sol::object get_hand_stack(sol::this_state s);

    sol::object get_wield_list(sol::this_state s);
    sol::object get_wield_stack(sol::this_state s);
    void set_wield_list(sol::optional<sol::object> list);

    unsigned int get_wield_index();
    void set_wield_index(unsigned int index);

    void set_flying(bool shouldFly);
    bool get_flying();
};

namespace ClientApi {
    static void local_player(sol::state& lua) {
        lua.new_usertype<LocalLuaPlayer>("LocalPlayer",
            "is_player", sol::property([]() { return true; }),

            "get_pos", &LocalLuaPlayer::get_pos,
            "get_block_pos", &LocalLuaPlayer::get_block_pos,
            "set_pos", &LocalLuaPlayer::set_pos,
            "get_vel", &LocalLuaPlayer::get_vel,
            "set_vel", &LocalLuaPlayer::set_vel,
            "get_look_yaw", &LocalLuaPlayer::get_look_yaw,
            "set_look_yaw", &LocalLuaPlayer::set_look_yaw,
            "get_look_pitch", &LocalLuaPlayer::get_look_pitch,
            "set_look_pitch", &LocalLuaPlayer::set_look_pitch,

            "get_inventory", &LocalLuaPlayer::get_inventory,
            "get_hand_list", &LocalLuaPlayer::get_hand_list,
            "get_hand_stack", &LocalLuaPlayer::get_hand_stack,
            "get_wield_list", &LocalLuaPlayer::get_wield_list,
            "set_wield_list", &LocalLuaPlayer::set_wield_list,
            "get_wield_index", &LocalLuaPlayer::get_wield_index,
            "set_wield_index", &LocalLuaPlayer::set_wield_index,
            "get_wield_stack", &LocalLuaPlayer::get_wield_stack,

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