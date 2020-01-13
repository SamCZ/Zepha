//
// Created by aurailus on 2019-12-24.
//

#include "ServerEntity.h"

void ServerEntity::setPos(glm::vec3 position) {
    this->position = position;
    this->dirty = true;
}

glm::vec3 ServerEntity::getPos() {
    return position;
}

void ServerEntity::setVisualOffset(glm::vec3 vs) {
    this->visualOffset = vs;
    this->dirty = true;
}

glm::vec3 ServerEntity::getVisualOffset(){
    return visualOffset;
}

void ServerEntity::setRotateX(float rotation) {
    this->rotation.x = rotation;
}

float ServerEntity::getRotateX() {
    return rotation.x;
}

void ServerEntity::setRotateY(float rotation) {
    this->rotation.y = rotation;
}

float ServerEntity::getRotateY() {
    return rotation.y;
}

void ServerEntity::setRotateZ(float rotation) {
    this->rotation.z = rotation;
}

float ServerEntity::getRotateZ() {
    return rotation.z;
}

void ServerEntity::setScale(float scale) {
    this->scale = scale;
    this->dirty = true;
}

float ServerEntity::getScale() {
    return scale;
}

void ServerEntity::setAppearance(std::string displayMode, std::string displayArgument1, std::string displayArgument2) {
    this->displayMode = displayMode;
    this->displayArgument1 = displayArgument1;
    this->displayArgument2 = displayArgument2;
    this->dirty = true;
}

bool ServerEntity::checkAndResetDirty() {
    bool dirty = this->dirty;
    this->dirty = false;
    return dirty;
}

Packet ServerEntity::createPacket(bool reliable) {
    return std::move(Serializer()
            .append(id)
            .append(position)
            .append(visualOffset)
            .append(rotation)
            .append(scale)
            .append(displayMode)
            .append(displayArgument1)
            .append(displayArgument2)
            .packet(PacketType::ENTITY_INFO, reliable));
}
