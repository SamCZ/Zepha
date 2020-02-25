//
// Created by aurailus on 2019-12-28.
//

#include "ServerLuaEntity.h"

void ServerLuaEntity::snap_pos(const sol::table &pos) {
    entity->setPos({pos["x"], pos["y"], pos["z"]});
}

void ServerLuaEntity::set_pos(const sol::table &pos) {
    entity->setPos({pos["x"], pos["y"], pos["z"]});
}

sol::table ServerLuaEntity::get_pos(sol::this_state s) {
    glm::vec3 pos = entity->getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

void ServerLuaEntity::snap_visual_offset(const sol::table &vs) {
    entity->setVisualOffset({vs["x"], vs["y"], vs["z"]});
}

void ServerLuaEntity::set_visual_offset(const sol::table &pos) {
    entity->setVisualOffset({pos["x"], pos["y"], pos["z"]});
}

sol::table ServerLuaEntity::get_visual_offset(sol::this_state s) {
    glm::vec3 v = entity->getVisualOffset();
    return sol::state_view(s).create_table_with("x", v.x, "y", v.y, "z", v.z);
}

void ServerLuaEntity::snap_pitch(float rot) {
    entity->setRotateX(rot);
}

void ServerLuaEntity::set_pitch(float rot) {
    entity->setRotateX(rot);
}

float ServerLuaEntity::get_pitch() {
    return entity->getRotateX();
}

void ServerLuaEntity::snap_yaw(float rot) {
    entity->setRotateY(rot);
}

void ServerLuaEntity::set_yaw(float rot) {
    entity->setRotateY(rot);
}

float ServerLuaEntity::get_yaw() {
    return entity->getRotateY();
}

void ServerLuaEntity::snap_roll(float rot) {
    entity->setRotateZ(rot);
}

void ServerLuaEntity::set_roll(float rot) {
    entity->setRotateZ(rot);
}

float ServerLuaEntity::get_roll() {
    return entity->getRotateZ();
}

void ServerLuaEntity::snap_scale(float scale) {
    entity->setScale(scale);
}

void ServerLuaEntity::set_scale(float scale) {
    entity->setScale(scale);
}

float ServerLuaEntity::get_scale() {
    return entity->getScale();
}

void ServerLuaEntity::set_display_type(const std::string &type, const std::string &arg, sol::optional<std::string> arg2) {
    if (strncmp(type.data(), "gameobject", 10) == 0) {
        ItemDef& def = defs.defs.fromStr(arg);
//        if (def.index == 0) throw "Invalid gameobject to display";

        if (def.type == ItemDef::Type::BLOCK)
            entity->setAppearance("gameobject", "block", arg);
        else if (def.type == ItemDef::Type::CRAFTITEM)
            entity->setAppearance("gameobject", "craftitem", arg);
    }
    else if (strncmp(type.data(), "model", 5) == 0 && arg2 && !arg2->empty()) {
        entity->setAppearance("model", arg, *arg2);
    }
}