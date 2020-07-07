//
// Created by aurailus on 2019-10-19.
//

#include <glm/glm.hpp>

#include "LocalLuaPlayer.h"

#include "../../LuaParser.h"
#include "../../../game/scene/world/Player.h"

void LocalLuaPlayer::set_pos(const sol::table &pos) {
    player.setPos({pos[1], pos[2], pos[3]});
}

sol::table LocalLuaPlayer::get_pos(sol::this_state s) {
    glm::vec3 pos = player.getPos();
    return LuaParser::luaVec(sol::state_view(s), pos);
}

sol::table LocalLuaPlayer::get_block_pos(sol::this_state s) {
    glm::vec3 pos = glm::floor(player.getPos());
    return LuaParser::luaVec(sol::state_view(s), pos);
}

void LocalLuaPlayer::set_vel(const sol::table &vel) {
    player.setVel({vel[1], vel[2], vel[3]});
}

sol::table LocalLuaPlayer::get_vel(sol::this_state s) {
    glm::vec3 vel = player.getVel();
    return LuaParser::luaVec(sol::state_view(s), vel);
}

void LocalLuaPlayer::set_look_yaw(float rot) {
    player.setYaw(rot);
}

float LocalLuaPlayer::get_look_yaw() {
    return player.getYaw();
}

void LocalLuaPlayer::set_look_pitch(float rot) {
    player.setPitch(rot);
}

float LocalLuaPlayer::get_look_pitch() {
    return player.getPitch();
}

bool LocalLuaPlayer::is_in_menu() {
    return player.isInMenu();
}

void LocalLuaPlayer::show_menu(std::shared_ptr<LuaGuiElement> root) {
    player.showMenu(root);
}

std::shared_ptr<LuaGuiElement> LocalLuaPlayer::get_hud() {
    return player.getHud();
}

void LocalLuaPlayer::set_hud(std::shared_ptr<LuaGuiElement> hud) {
    player.setHud(hud);
}

void LocalLuaPlayer::close_menu() {
    player.closeMenu();
}

LocalLuaInventory LocalLuaPlayer::get_inventory() {
    return LocalLuaInventory(player.getInventory());
}

void LocalLuaPlayer::set_selected_block(std::string block) {
    player.setActiveBlock(block);
}

void LocalLuaPlayer::set_flying(bool shouldFly) {
    player.setFlying(shouldFly);
}

bool LocalLuaPlayer::get_flying() {
    return player.isFlying();
}
