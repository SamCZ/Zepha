//
// Created by aurailus on 2019-12-24.
//

#include <iostream>

#include "ServerLuaEntity.h"

#include "util/net/Serializer.h"

void ServerLuaEntity::setPos(glm::vec3 position) {
    Entity::setPos(position);
    dirtyFields.emplace(NetField::POS);
}

void ServerLuaEntity::setVel(glm::vec3 vel) {
    Entity::setVel(rot);
    dirtyFields.emplace(NetField::VEL);
}

void ServerLuaEntity::setRot(glm::vec3 rot) {
    Entity::setRot(rot);
    dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setScale(float scale) {
    Entity::setScale(scale);
    dirtyFields.emplace(NetField::SCALE);
}

void ServerLuaEntity::setScale(glm::vec3 scale) {
    Entity::setScale(scale);
    dirtyFields.emplace(NetField::SCALE);
}

void ServerLuaEntity::setRotateX(float rotation) {
    Entity::setRotateX(rotation);
    dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setRotateY(float rotation) {
    Entity::setRotateY(rotation);
    dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setRotateZ(float rotation) {
    Entity::setRotateZ(rotation);
    dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setVisualOffset(glm::vec3 vs) {
    Entity::setVisualOffset(vs);
    dirtyFields.emplace(NetField::VISUAL_OFF);
}

void ServerLuaEntity::setAppearance(const std::string& dMode, const std::string& argA, const std::string& argB) {
    this->dMode = dMode;
    this->dArgA = argA;
    this->dArgB = argB;
    dirtyFields.emplace(NetField::DISPLAY);
}

void ServerLuaEntity::dirtyField(NetField field) {
    dirtyFields.emplace(field);
}

std::string ServerLuaEntity::serialize() {
    if (dirtyFields.empty() && !fullSend) return {};

    Serializer s;
    s.append<unsigned int>(id);

    if (dirtyFields.count(NetField::POS)        || fullSend) s.appendE(NetField::POS)       .append(pos);
    if (dirtyFields.count(NetField::VEL)        || fullSend) s.appendE(NetField::VEL)       .append(vel);
    if (dirtyFields.count(NetField::SCALE)      || fullSend) s.appendE(NetField::SCALE)     .append(scale);
    if (dirtyFields.count(NetField::ROT)        || fullSend) s.appendE(NetField::ROT)       .append(rot);
    if (dirtyFields.count(NetField::VISUAL_OFF) || fullSend) s.appendE(NetField::VISUAL_OFF).append(visualOff);
    if (dirtyFields.count(NetField::ANIM_STATE) || fullSend) s.appendE(NetField::ANIM_STATE).append<bool>(animation.isPlaying());
    if (dirtyFields.count(NetField::DISPLAY)    || fullSend) s.appendE(NetField::DISPLAY)   .append(dMode).append(dArgA).append(dArgB);
    if (dirtyFields.count(NetField::ANIM_RANGE) || fullSend) s.appendE(NetField::ANIM_RANGE)
        .append<unsigned int>(animation.getBounds().x).append<unsigned int>(animation.getBounds().y).append<bool>(animation.isLooping());

    dirtyFields.clear();
    fullSend = false;

    return s.data;
}