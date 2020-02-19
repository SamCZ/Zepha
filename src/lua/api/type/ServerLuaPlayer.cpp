//
// Created by aurailus on 2020-02-17.
//

#include "ServerLuaPlayer.h"
#include "../../../util/net/NetHandler.h"

void ServerLuaPlayer::set_pos(const sol::table &pos) {
    player.setPos({pos["x"], pos["y"], pos["z"]});
}

sol::table ServerLuaPlayer::get_pos(sol::this_state s) {
    glm::vec3 pos = player.getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

sol::table ServerLuaPlayer::get_block_pos(sol::this_state s) {
    glm::vec3 pos = glm::floor(player.getPos());
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

//void ServerLuaPlayer::set_vel(const sol::table &vel) {
//    player.setVel({vel["x"], vel["y"], vel["z"]});
//}

//sol::table ServerLuaPlayer::get_vel(sol::this_state s) {
//    glm::vec3 vel = player.getVel();
//    return sol::state_view(s).create_table_with("x", vel.x, "y", vel.y, "z", vel.z);
//}

void ServerLuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot);
}

float ServerLuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void ServerLuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot);
}

float ServerLuaPlayer::get_look_pitch() {
    return player.getPitch();
}

void ServerLuaPlayer::set_flying(bool shouldFly) {
    player.setFlying(shouldFly);
}

bool ServerLuaPlayer::get_flying() {
    return player.isFlying();
}

std::string ServerLuaPlayer::get_name() {
    return player.username;
}

unsigned int ServerLuaPlayer::get_cid() {
    return cid;
}

std::string ServerLuaPlayer::get_address() {
    return NetHandler::intToIPString(player.address.host) + ":" + std::to_string(player.address.port);
}

LuaInventory ServerLuaPlayer::get_inventory() {
    return LuaInventory(player.getInventory());
}
