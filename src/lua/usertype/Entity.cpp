//
// Created by aurailus on 2020-08-04.
//

#include "Entity.h"

#include "../../world/Dimension.h"
#include "../../def/LocalSubgame.h"
#include "../../def/item/BlockDef.h"
#include "../../def/DefinitionAtlas.h"
#include "../../def/item/CraftItemDef.h"
#include "../../game/scene/world/World.h"
#include "../../net/server/world/ServerLuaEntity.h"

sol::object Api::Usertype::Entity::get_animation_manager(sol::this_state s) {
    if (entity.isL()) return sol::make_object(s, std::static_pointer_cast<LocalAnimationManager>(animation));
    else return sol::make_object(s, std::static_pointer_cast<ServerAnimationManager>(animation));
}

unsigned int Api::Usertype::Entity::get_id() {
    return entity->getId();
}

void Api::Usertype::Entity::snap_pos(glm::vec3 pos) {
    entity->setPos(pos);
}

void Api::Usertype::Entity::set_pos(glm::vec3 pos) {
    entity->setPos(pos);
}

glm::vec3 Api::Usertype::Entity::get_pos() {
    return entity->getPos();
}

void Api::Usertype::Entity::snap_visual_offset(glm::vec3 vs) {
    entity->setVisualOffset(vs);
}

void Api::Usertype::Entity::set_visual_offset(glm::vec3 vs) {
    entity->setVisualOffset(vs);
}

glm::vec3 Api::Usertype::Entity::get_visual_offset() {
    return entity->getVisualOffset();
}

void Api::Usertype::Entity::snap_pitch(float rot) {
    entity->setRotateX(rot);
}

void Api::Usertype::Entity::set_pitch(float rot) {
    entity->setRotateX(rot);
}

float Api::Usertype::Entity::get_pitch() {
    return entity->getRotateX();
}

void Api::Usertype::Entity::snap_yaw(float rot) {
    entity->setRotateY(rot);
}

void Api::Usertype::Entity::set_yaw(float rot) {
    entity->setRotateY(rot);
}

float Api::Usertype::Entity::get_yaw() {
    return entity->getRotateY();
}

void Api::Usertype::Entity::snap_roll(float rot) {
    entity->setRotateZ(rot);
}

void Api::Usertype::Entity::set_roll(float rot) {
    entity->setRotateZ(rot);
}

float Api::Usertype::Entity::get_roll() {
    return entity->getRotateZ();
}

void Api::Usertype::Entity::snap_scale(float scale) {
    entity->setScale(scale);
}

void Api::Usertype::Entity::set_scale(float scale) {
    entity->setScale(scale);
}

float Api::Usertype::Entity::get_scale() {
    return entity->getScale().x;
}

Api::Usertype::Dimension Api::Usertype::Entity::get_dimension() {
    return Dimension(entity->getDim());
}

void Api::Usertype::Entity::set_dimension(const std::string& identifier) {
    entity->setDim(entity->getDim()->getWorld().getDimension(identifier));
}

void Api::Usertype::Entity::set_display_type(const std::string &type, const std::string &arg, sol::optional<std::string> arg2) {
    if (strncmp(type.data(), "gameobject", 10) == 0) {
        ItemDef& def = entity->getGame()->getDefs().fromStr(arg);
        if (def.type == ItemDef::Type::BLOCK) entity->setAppearance("gameobject", "block", arg);
        else if (def.type == ItemDef::Type::CRAFTITEM) entity->setAppearance("gameobject", "craftitem", arg);
    }
    else if (strncmp(type.data(), "model", 5) == 0 && arg2 && !arg2->empty()) {
        entity->setAppearance("model", arg, *arg2);
    }
    else throw std::runtime_error("Invalid display type parameters.");
}

void Api::Usertype::Entity::bind(State, sol::state &lua, sol::table &core) {
    lua.new_usertype<Entity>("Entity",
        "get_id", &Entity::get_id,
        "snap_pos", &Entity::snap_pos,
        "set_pos", &Entity::set_pos,
        "get_pos", &Entity::get_pos,
        "snap_visual_offset", &Entity::snap_visual_offset,
        "set_visual_offset", &Entity::set_visual_offset,
        "get_visual_offset", &Entity::get_visual_offset,
        "snap_pitch", &Entity::snap_pitch,
        "set_pitch", &Entity::set_pitch,
        "get_pitch", &Entity::get_pitch,
        "snap_yaw", &Entity::snap_yaw,
        "set_yaw", &Entity::set_yaw,
        "get_yaw", &Entity::get_yaw,
        "snap_roll", &Entity::snap_roll,
        "set_roll", &Entity::set_roll,
        "get_roll", &Entity::get_roll,
        "snap_scale", &Entity::snap_scale,
        "set_scale", &Entity::set_scale,
        "get_scale", &Entity::get_scale,

        "set_display_type", &Entity::set_display_type,

        "id", sol::property(&Entity::get_id),
        "pos", sol::property(&Entity::get_pos, &Entity::set_pos),
        "visual_offset", sol::property(&Entity::get_visual_offset, &Entity::set_visual_offset),
        "pitch", sol::property(&Entity::get_pitch, &Entity::set_pitch),
        "yaw", sol::property(&Entity::get_yaw, &Entity::set_yaw),
        "roll", sol::property(&Entity::get_roll, &Entity::set_roll),
        "scale", sol::property(&Entity::get_scale, &Entity::set_scale),
        "dim", sol::property(&Entity::get_dimension),

        "anims", sol::property(&Entity::get_animation_manager)
    );
}