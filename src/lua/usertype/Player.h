//
// Created by aurailus on 2019-10-19.
//

#pragma once

//#include <sol/forward.hpp>

#include <memory>

#include "Inventory.h"
#include "Dimension.h"
#include "world/player/LocalPlayer.h"

class LuaGuiElement;

namespace Api::Usertype {
    class ServerPlayer : public SubgameUsertype {
    public:
        ServerPlayer(PlayerPtr player) : player(player) {}

        PlayerPtr player;

        unsigned int get_id();

        glm::vec3 get_pos();
        glm::vec3 get_block_pos();
        void set_pos(glm::vec3 pos);

        glm::vec3 get_vel();
        void set_vel(glm::vec3 vel);

        float get_look_yaw();
        void set_look_yaw(float rot);

        float get_look_pitch();
        void set_look_pitch(float rot);

        sol::object get_hand_list(sol::this_state s);
        sol::object get_hand_stack(sol::this_state s);
        void set_hand_list(sol::optional<sol::object> list);

        sol::object get_wield_list(sol::this_state s);
        sol::object get_wield_stack(sol::this_state s);
        void set_wield_list(sol::optional<sol::object> list);

        Inventory get_inventory();

        Dimension get_dimension();
        void set_dimension(const std::string& identifier);

        unsigned int get_wield_index();
        void set_wield_index(unsigned int index);

        void set_flying(bool shouldFly);
        bool get_flying();

        static void bind(State state, sol::state& lua, sol::table& core);
    };

    class LocalPlayer : public ServerPlayer {
    public:
        LocalPlayer(PlayerPtr player) : ServerPlayer(player) {}

        bool is_in_menu();
        void show_menu(std::shared_ptr<LuaGuiElement> root);
        void close_menu();

        std::shared_ptr<LuaGuiElement> get_hud();
        void set_hud(std::shared_ptr<LuaGuiElement> hud);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}