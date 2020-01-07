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

void ServerEntity::setAngle(float angle) {
    this->angle = angle;
    this->dirty = true;
}

float ServerEntity::getAngle() {
    return angle;
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

void ServerEntity::fillPacket(Packet &p) {
    p.data = Serializer()
            .append(id)
            .append(position)
            .append(visualOffset)
            .append(angle)
            .append(scale)
            .append(displayMode)
            .append(displayArgument1)
            .append(displayArgument2)
            .data;
}
