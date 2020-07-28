//
// Created by aurailus on 2020-02-17.
//

#include "ServerLuaPlayer.h"

#include "../LuaParser.h"
#include "../../net/NetHandler.h"
#include "ServerLuaInventoryList.h"

std::string ServerLuaPlayer::get_name() {
    return player.username;
}

unsigned int ServerLuaPlayer::get_cid() {
    return cid;
}

std::string ServerLuaPlayer::get_address() {
    return NetHandler::intToIPString(player.address.host) + ":" + std::to_string(player.address.port);
}

glm::vec3 ServerLuaPlayer::get_pos() {
    return player.getPos();
}

glm::vec3 ServerLuaPlayer::get_block_pos() {
    return glm::floor(player.getPos());
}

void ServerLuaPlayer::set_pos(glm::vec3 pos) {
    player.setPos(pos, true);
}

glm::vec3 ServerLuaPlayer::get_vel() {
    return player.getVel();
}

void ServerLuaPlayer::set_vel(glm::vec3 vel) {
    player.setVel(vel, true);
}

float ServerLuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void ServerLuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot, true);
}

float ServerLuaPlayer::get_look_pitch() {
    return player.getPitch();
}

void ServerLuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot, true);
}

bool ServerLuaPlayer::get_flying() {
    return player.isFlying();
}

void ServerLuaPlayer::set_flying(bool shouldFly) {
    player.setFlying(shouldFly, true);
}

ServerLuaInventory ServerLuaPlayer::get_inventory() {
    return ServerLuaInventory(*player.getInventory());
}

sol::object ServerLuaPlayer::get_hand_list(sol::this_state s) {
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*player.getHandList()));
}

void ServerLuaPlayer::set_hand_list(sol::object list) {
    if (!list) player.setHandList(nullptr);
    else if (list.is<std::string>()) player.setHandList(list.as<std::string>(), true);
    else if (list.is<ServerLuaInventoryList>()) player.setHandList(list.as<ServerLuaInventoryList>().get_name(), true);
    else throw "Attempted to set hand list to nil.";
}

sol::object ServerLuaPlayer::get_hand_stack(sol::this_state s) {
    auto list = player.getHandList();
    if (!list) return sol::nil;
    return sol::make_object<LuaItemStack>(s, ServerLuaInventoryList(*list).get_stack(1));
}

sol::object ServerLuaPlayer::get_wield_list(sol::this_state s) {
    auto list = player.getWieldList();
    if (!list) return sol::nil;
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*list));
}

void ServerLuaPlayer::set_wield_list(sol::object list) {
    if (!list) player.setWieldList(nullptr, true);
    else if (list.is<std::string>()) player.setWieldList(list.as<std::string>(), true);
    else if (list.is<ServerLuaInventoryList>()) player.setWieldList(list.as<ServerLuaInventoryList>().get_name(), true);
    else throw "Attempted to set wield list to nil.";
}

sol::object ServerLuaPlayer::get_wield_stack(sol::this_state s) {
    auto list = player.getWieldList();
    if (!list) return sol::nil;
    return sol::make_object<LuaItemStack>(s, ServerLuaInventoryList(*list).get_stack(player.getWieldIndex() + 1));
}

unsigned int ServerLuaPlayer::get_wield_index() {
    return player.getWieldIndex() + 1;
}

void ServerLuaPlayer::set_wield_index(unsigned int index) {
    player.setWieldIndex(index - 1, true);
}
