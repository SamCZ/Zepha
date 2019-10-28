//
// Created by aurailus on 2019-10-19.
//

#include "LuaPlayer.h"

void LuaPlayer::set_pos(const sol::table &pos) {
    player.setPos({pos["x"], pos["y"], pos["z"]});
}

sol::table LuaPlayer::get_pos(sol::this_state s) {
    glm::vec3 pos = player.getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

sol::table LuaPlayer::get_block_pos(sol::this_state s) {
    glm::vec3 pos = TransPos::roundPos(player.getPos());
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

void LuaPlayer::set_vel(const sol::table &vel) {
    player.setVel({vel["x"], vel["y"], vel["z"]});
}

sol::table LuaPlayer::get_vel(sol::this_state s) {
    glm::vec3 vel = player.getVel();
    return sol::state_view(s).create_table_with("x", vel.x, "y", vel.y, "z", vel.z);
}

void LuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot);
}

float LuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void LuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot);
}

float LuaPlayer::get_look_pitch() {
    return player.getPitch();
}

std::string LuaPlayer::get_menu_state() {
    return player.getGui().getMenuState();
}

void LuaPlayer::set_selected_block(std::string block) {
    player.setActiveBlock(block);
}
