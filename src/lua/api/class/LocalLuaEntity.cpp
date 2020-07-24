//
// Created by aurailus on 03/10/19.
//

#include "LocalLuaEntity.h"

#include "../../Lua.h"
#include "../../LuaParser.h"
#include "../../../def/ClientGame.h"
#include "../../../def/item/BlockDef.h"
#include "../../../def/item/CraftItemDef.h"

void LocalLuaEntity::snap_pos(glm::vec3 pos) {
    entity->setPos(pos);
}

void LocalLuaEntity::set_pos(glm::vec3 pos) {
    entity->interpPos(pos);
}

glm::vec3 LocalLuaEntity::get_pos() {
    return entity->getPos();
}

void LocalLuaEntity::snap_visual_offset(glm::vec3 vs) {
    entity->setVisualOffset(vs);
}

void LocalLuaEntity::set_visual_offset(glm::vec3 vs) {
    entity->interpVisualOffset(vs);
}

glm::vec3 LocalLuaEntity::get_visual_offset() {
    return entity->getVisualOffset();
}

void LocalLuaEntity::snap_pitch(float rot) {
    entity->setRotateX(rot);
}

void LocalLuaEntity::set_pitch(float rot) {
    entity->interpRotateX(rot);
}

float LocalLuaEntity::get_pitch() {
    return entity->getRotateX();
}

void LocalLuaEntity::snap_yaw(float rot) {
    entity->setRotateY(rot);
}

void LocalLuaEntity::set_yaw(float rot) {
    entity->interpRotateY(rot);
}

float LocalLuaEntity::get_yaw() {
    return entity->getRotateY();
}

void LocalLuaEntity::snap_roll(float rot) {
    entity->setRotateZ(rot);
}

void LocalLuaEntity::set_roll(float rot) {
    entity->interpRotateZ(rot);
}

float LocalLuaEntity::get_roll() {
    return entity->getRotateZ();
}

void LocalLuaEntity::snap_scale(float scale) {
    entity->setScale(scale);
}

void LocalLuaEntity::set_scale(float scale) {
    entity->interpScale(scale);
}

float LocalLuaEntity::get_scale() {
    return entity->getScale().x;
}

void LocalLuaEntity::set_display_type(const std::string &type, const std::string &arg, sol::optional<std::string> arg2) {
    if (strncmp(type.data(), "gameobject", 10) == 0) {
        ItemDef& def = defs.defs.fromStr(arg);

        if (def.type == ItemDef::Type::BLOCK)
            entity->setModel(static_cast<BlockDef&>(def).entityModel);
        else if (def.type == ItemDef::Type::CRAFTITEM)
            entity->setModel(static_cast<CraftItemDef&>(def).entityModel);
    }
    else if (strncmp(type.data(), "model", 5) == 0 && arg2 && !arg2->empty()) {
        auto model = std::make_shared<Model>();
        model->fromSerialized(defs.models.models[arg], {defs.textures[*arg2]});
        entity->setModel(model);
    }
}