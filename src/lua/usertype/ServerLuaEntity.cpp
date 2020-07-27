//
// Created by aurailus on 2019-12-28.
//

#include "ServerLuaEntity.h"

#include "../LuaParser.h"
#include "../../def/ItemDef.h"
#include "../../def/ServerSubgame.h"
#include "../../def/ServerDefinitionAtlas.h"

void ServerLuaEntity::snap_pos(glm::vec3 pos) {
    entity->setPos(pos);
}

void ServerLuaEntity::set_pos(glm::vec3 pos) {
    entity->setPos(pos);
}

glm::vec3 ServerLuaEntity::get_pos() {
    return entity->getPos();
}

void ServerLuaEntity::snap_visual_offset(glm::vec3 vs) {
    entity->setVisualOffset(vs);
}

void ServerLuaEntity::set_visual_offset(glm::vec3 vs) {
    entity->setVisualOffset(vs);
}

glm::vec3 ServerLuaEntity::get_visual_offset() {
    return entity->getVisualOffset();
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
        ItemDef& def = defs.defs->fromStr(arg);
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