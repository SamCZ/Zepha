//
// Created by aurailus on 2019-10-19.
//

#include "LuaLocalPlayer.h"

void LuaLocalPlayer::set_pos(const sol::table &pos) {
    player.setPos({pos["x"], pos["y"], pos["z"]});
}

sol::table LuaLocalPlayer::get_pos(sol::this_state s) {
    glm::vec3 pos = player.getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

sol::table LuaLocalPlayer::get_block_pos(sol::this_state s) {
    glm::vec3 pos = glm::floor(player.getPos());
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

void LuaLocalPlayer::set_vel(const sol::table &vel) {
    player.setVel({vel["x"], vel["y"], vel["z"]});
}

sol::table LuaLocalPlayer::get_vel(sol::this_state s) {
    glm::vec3 vel = player.getVel();
    return sol::state_view(s).create_table_with("x", vel.x, "y", vel.y, "z", vel.z);
}

void LuaLocalPlayer::set_look_yaw(float rot) {
    player.setYaw(rot);
}

float LuaLocalPlayer::get_look_yaw() {
    return player.getYaw();
}

void LuaLocalPlayer::set_look_pitch(float rot) {
    player.setPitch(rot);
}

float LuaLocalPlayer::get_look_pitch() {
    return player.getPitch();
}

std::string LuaLocalPlayer::get_menu_state() {
    return player.getMenuState();
}

void LuaLocalPlayer::open_menu(sol::this_state s, std::string menu, sol::optional<sol::table> callbacks) {
    if (callbacks) {
        std::map<std::string, GuiBuilder::ComponentCallbacks> callbackMap;

        for (auto& pair : *callbacks)  {
            if (!pair.first.is<std::string>() || !pair.second.is<sol::table>()) continue;
            std::string identifier = pair.first.as<std::string>();
            sol::table callbacks = pair.second.as<sol::table>();
            GuiBuilder::ComponentCallbacks componentCallbacks {};

            auto left  = callbacks.get<sol::optional<sol::function>>("left");
            auto right = callbacks.get<sol::optional<sol::function>>("right");
            auto hover = callbacks.get<sol::optional<sol::function>>("hover");

            if (left) componentCallbacks.left = [=](bool down, glm::ivec2 pos) {
                    (*left)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            if (right) componentCallbacks.right = [=](bool down, glm::ivec2 pos) {
                    (*right)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            if (hover) componentCallbacks.hover = [=](bool down, glm::ivec2 pos) {
                    (*hover)(down, sol::state_view(s).create_table_with("x", pos.x, "y", pos.y)); };

            callbackMap.emplace(identifier, componentCallbacks);
        }

        player.setMenu(menu, callbackMap);
    }
    else player.setMenu(menu, {});
}

void LuaLocalPlayer::close_menu() {
    player.closeMenu();
}

LuaInventory LuaLocalPlayer::get_inventory() {
    return LuaInventory(player.getInventory());
}

void LuaLocalPlayer::set_selected_block(std::string block) {
    player.setActiveBlock(block);
}
