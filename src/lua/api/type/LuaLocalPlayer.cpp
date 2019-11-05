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
    glm::vec3 pos = TransPos::roundPos(player.getPos());
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

void LuaLocalPlayer::open_menu(std::string menu) {
    player.setMenu(menu);
}

void LuaLocalPlayer::close_menu() {
    player.closeMenu();
}

void LuaLocalPlayer::set_selected_block(std::string block) {
    player.setActiveBlock(block);
}