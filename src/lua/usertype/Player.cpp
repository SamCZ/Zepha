//
// Created by aurailus on 2019-10-19.
//

#include <glm/glm.hpp>

#include "Player.h"

#include "ItemStack.h"
#include "InventoryList.h"
#include "../../net/server/world/ServerWorld.h"
#include "../../game/scene/world/LocalWorld.h"
#include "../../game/scene/world/LocalPlayer.h"

unsigned int Api::Usertype::ServerPlayer::get_id() {
    return player->getId();
}

glm::vec3 Api::Usertype::ServerPlayer::get_pos() {
    return player->getPos();
}

glm::vec3 Api::Usertype::ServerPlayer::get_block_pos() {
    return glm::floor(player->getPos());
}

void Api::Usertype::ServerPlayer::set_pos(glm::vec3 pos) {
    player->setPos(pos, true);
}

glm::vec3 Api::Usertype::ServerPlayer::get_vel() {
    return player->getVel();
}

void Api::Usertype::ServerPlayer::set_vel(glm::vec3 vel) {
    player->setVel(vel, true);
}

float Api::Usertype::ServerPlayer::get_look_yaw() {
    return player->getYaw();
}

void Api::Usertype::ServerPlayer::set_look_yaw(float rot) {
    player->setYaw(rot, true);
}

float Api::Usertype::ServerPlayer::get_look_pitch() {
    return player->getPitch();
}

void Api::Usertype::ServerPlayer::set_look_pitch(float rot) {
    player->setPitch(rot, true);
}

Api::Usertype::Inventory Api::Usertype::ServerPlayer::get_inventory() {
    return Inventory(player->getInventory());
}

Api::Usertype::Dimension Api::Usertype::ServerPlayer::get_dimension() {
    return Dimension(player->getDim());
}

void Api::Usertype::ServerPlayer::set_dimension(const std::string& identifier) {
    player->setDim(player->getDim()->getWorld().getDimension(identifier));
}

sol::object Api::Usertype::ServerPlayer::get_hand_list(sol::this_state s) {
    auto listStr = player->getHandList();
    if (listStr.empty()) return sol::nil;
    return sol::make_object<InventoryList>(s, InventoryList(player->getInventory()->getList(listStr)));
}

sol::object Api::Usertype::ServerPlayer::get_hand_stack(sol::this_state s) {
    auto listStr = player->getHandList();
    if (listStr.empty()) return sol::nil;
    return sol::make_object<ItemStack>(s, InventoryList(player->getInventory()->getList(listStr)).get_stack(1));
}

void Api::Usertype::ServerPlayer::set_hand_list(sol::optional<sol::object> list) {
    if (!list) player->setHandList("", true);
    else if (list->is<std::string>()) player->setHandList(list->as<std::string>(), true);
    else if (list->is<InventoryList>()) player->setHandList(list->as<InventoryList>().get_name(), true);
    else throw "Attempted to set hand list to nil.";
}

sol::object Api::Usertype::ServerPlayer::get_wield_list(sol::this_state s) {
    auto listStr = player->getWieldList();
    if (listStr.empty()) return sol::nil;
    return sol::make_object<InventoryList>(s, InventoryList(player->getInventory()->getList(listStr)));
}

void Api::Usertype::ServerPlayer::set_wield_list(sol::optional<sol::object> list) {
    if (!list) player->setWieldList("", true);
    else if (list->is<std::string>()) player->setWieldList(list->as<std::string>(), true);
    else if (list->is<InventoryList>()) player->setWieldList(list->as<InventoryList>().get_name(), true);
    else throw "Attempted to set wield list to nil.";
}

unsigned int Api::Usertype::ServerPlayer::get_wield_index() {
    return player->getWieldIndex() + 1;
}

void Api::Usertype::ServerPlayer::set_wield_index(unsigned int index) {
    player->setWieldIndex(index - 1, true);
}

sol::object Api::Usertype::ServerPlayer::get_wield_stack(sol::this_state s) {
    auto listStr = player->getWieldList();
    if (listStr.empty()) return sol::nil;
    return sol::make_object<ItemStack>(s, InventoryList(player->getInventory()
        ->getList(listStr)).get_stack(player->getWieldIndex() + 1));
}

void Api::Usertype::ServerPlayer::set_flying(bool shouldFly) {
    player->setFlying(shouldFly, true);
}

bool Api::Usertype::ServerPlayer::get_flying() {
    return player->isFlying();
}

void Api::Usertype::ServerPlayer::bind(State, sol::state &lua, sol::table &core) {
    lua.new_usertype<ServerPlayer>("Player",
        "get_id", &ServerPlayer::get_id,
        "get_pos", &ServerPlayer::get_pos,
        "get_block_pos", &ServerPlayer::get_block_pos,
        "set_pos", &ServerPlayer::set_pos,
        "get_vel", &ServerPlayer::get_vel,
        "set_vel", &ServerPlayer::set_vel,
        "get_look_yaw", &ServerPlayer::get_look_yaw,
        "set_look_yaw", &ServerPlayer::set_look_yaw,
        "get_look_pitch", &ServerPlayer::get_look_pitch,
        "set_look_pitch", &ServerPlayer::set_look_pitch,

        "get_inventory", &ServerPlayer::get_inventory,
        "get_hand_list", &ServerPlayer::get_hand_list,
        "set_hand_list", &ServerPlayer::set_hand_list,
        "get_hand_stack", &ServerPlayer::get_hand_stack,
        "get_wield_list", &ServerPlayer::get_wield_list,
        "set_wield_list", &ServerPlayer::set_wield_list,
        "get_wield_index", &ServerPlayer::get_wield_index,
        "set_wield_index", &ServerPlayer::set_wield_index,
        "get_wield_stack", &ServerPlayer::get_wield_stack,

        "get_dimension", &ServerPlayer::get_dimension,
        "set_dimension", &ServerPlayer::set_dimension,

        "id", sol::property(&ServerPlayer::get_id),
        "pos", sol::property(&ServerPlayer::get_pos, &ServerPlayer::set_pos),
        "block_pos", sol::property(&ServerPlayer::get_block_pos, &ServerPlayer::set_pos),
        "vel", sol::property(&ServerPlayer::get_vel, &ServerPlayer::set_vel),
        "look_yaw", sol::property(&ServerPlayer::get_look_yaw, &ServerPlayer::set_look_yaw),
        "look_pitch", sol::property(&ServerPlayer::get_look_pitch, &ServerPlayer::set_look_pitch),
        "dim", sol::property(&ServerPlayer::get_dimension),

        "flying", sol::property(&ServerPlayer::set_flying, &ServerPlayer::get_flying)
    );
}

bool Api::Usertype::LocalPlayer::is_in_menu() {
    return player.l()->isInMenu();
}

void Api::Usertype::LocalPlayer::show_menu(std::shared_ptr<LuaGuiElement> root) {
    return player.l()->showMenu(root);
}

void Api::Usertype::LocalPlayer::close_menu() {
    return player.l()->closeMenu();
}

std::shared_ptr<LuaGuiElement> Api::Usertype::LocalPlayer::get_hud() {
    return player.l()->getHud();
}

void Api::Usertype::LocalPlayer::set_hud(std::shared_ptr<LuaGuiElement> hud) {
    player.l()->setHud(hud);
}

void Api::Usertype::LocalPlayer::bind(State, sol::state &lua, sol::table &core) {
    lua.new_usertype<LocalPlayer>("Player",
        "get_id", &LocalPlayer::get_id,
        "get_pos", &LocalPlayer::get_pos,
        "get_block_pos", &LocalPlayer::get_block_pos,
        "set_pos", &LocalPlayer::set_pos,
        "get_vel", &LocalPlayer::get_vel,
        "set_vel", &LocalPlayer::set_vel,
        "get_look_yaw", &LocalPlayer::get_look_yaw,
        "set_look_yaw", &LocalPlayer::set_look_yaw,
        "get_look_pitch", &LocalPlayer::get_look_pitch,
        "set_look_pitch", &LocalPlayer::set_look_pitch,

        "get_inventory", &LocalPlayer::get_inventory,
        "get_hand_list", &LocalPlayer::get_hand_list,
        "get_hand_stack", &LocalPlayer::get_hand_stack,
        "get_wield_list", &LocalPlayer::get_wield_list,
        "set_wield_list", &LocalPlayer::set_wield_list,
        "get_wield_index", &LocalPlayer::get_wield_index,
        "set_wield_index", &LocalPlayer::set_wield_index,
        "get_wield_stack", &LocalPlayer::get_wield_stack,

        "get_dimension", &LocalPlayer::get_dimension,

        "show_menu", &LocalPlayer::show_menu,
        "close_menu", &LocalPlayer::close_menu,
        "set_hud", &LocalPlayer::set_hud,
        "get_hud", &LocalPlayer::get_hud,

        "pos", sol::property(&LocalPlayer::get_pos, &LocalPlayer::set_pos),
        "block_pos", sol::property(&LocalPlayer::get_block_pos, &LocalPlayer::set_pos),
        "vel", sol::property(&LocalPlayer::get_vel, &LocalPlayer::set_vel),
        "look_yaw", sol::property(&LocalPlayer::get_look_yaw, &LocalPlayer::set_look_yaw),
        "look_pitch", sol::property(&LocalPlayer::get_look_pitch, &LocalPlayer::set_look_pitch),
        "dim", sol::property(&LocalPlayer::get_dimension),

        "flying", sol::property(&LocalPlayer::set_flying, &LocalPlayer::get_flying),

        "in_menu", sol::property(&LocalPlayer::is_in_menu)
    );
}