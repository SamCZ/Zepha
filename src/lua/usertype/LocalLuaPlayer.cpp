//
// Created by aurailus on 2019-10-19.
//

#include <glm/glm.hpp>

#include "LocalLuaPlayer.h"

#include "LuaItemStack.h"
#include "LocalLuaInventoryList.h"
#include "../../game/scene/world/Player.h"

glm::vec3 LocalLuaPlayer::get_pos() {
    return player.getPos();
}

glm::vec3 LocalLuaPlayer::get_block_pos() {
    return glm::floor(player.getPos());
}

void LocalLuaPlayer::set_pos(glm::vec3 pos) {
    player.setPos(pos, true);
}

glm::vec3 LocalLuaPlayer::get_vel() {
    return player.getVel();
}

void LocalLuaPlayer::set_vel(glm::vec3 vel) {
    player.setVel(vel, true);
}

float LocalLuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void LocalLuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot, true);
}

float LocalLuaPlayer::get_look_pitch() {
    return player.getPitch();
}

void LocalLuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot, true);
}

bool LocalLuaPlayer::is_in_menu() {
    return player.isInMenu();
}

void LocalLuaPlayer::show_menu(std::shared_ptr<LuaGuiElement> root) {
    player.showMenu(root);
}

void LocalLuaPlayer::close_menu() {
    player.closeMenu();
}

std::shared_ptr<LuaGuiElement> LocalLuaPlayer::get_hud() {
    return player.getHud();
}

void LocalLuaPlayer::set_hud(std::shared_ptr<LuaGuiElement> hud) {
    player.setHud(hud);
}

LocalLuaInventory LocalLuaPlayer::get_inventory() {
    return LocalLuaInventory(player.getInventory());
}

sol::object LocalLuaPlayer::get_hand_list(sol::this_state s) {
    auto list = player.getHandList();
    if (!list) return sol::nil;
    return sol::make_object<LocalLuaInventoryList>(s, LocalLuaInventoryList(*list));
}

sol::object LocalLuaPlayer::get_hand_stack(sol::this_state s) {
    auto list = player.getHandList();
    if (!list) return sol::nil;
    return sol::make_object<LuaItemStack>(s, LocalLuaInventoryList(*list).get_stack(1));
}

sol::object LocalLuaPlayer::get_wield_list(sol::this_state s) {
    auto list = player.getWieldList();
    if (!list) return sol::nil;
    return sol::make_object<LocalLuaInventoryList>(s, LocalLuaInventoryList(*list));
}

void LocalLuaPlayer::set_wield_list(sol::optional<sol::object> list) {
    if (!list) player.setWieldList("");
    else if (list->is<std::string>()) player.setWieldList(list->as<std::string>());
    else if (list->is<LocalLuaInventoryList>()) player.setWieldList(list->as<LocalLuaInventoryList>().get_name());
    else throw "Attempted to set wield list to nil.";
}

unsigned int LocalLuaPlayer::get_wield_index() {
    return player.getWieldIndex() + 1;
}

void LocalLuaPlayer::set_wield_index(unsigned int index) {
    player.setWieldIndex(index - 1);
}

sol::object LocalLuaPlayer::get_wield_stack(sol::this_state s) {
    auto list = player.getWieldList();
    if (!list) return sol::nil;
    return sol::make_object<LuaItemStack>(s, LocalLuaInventoryList(*list).get_stack(player.getWieldIndex() + 1));
}

void LocalLuaPlayer::set_flying(bool shouldFly) {
    player.setFlying(shouldFly);
}

bool LocalLuaPlayer::get_flying() {
    return player.isFlying();
}