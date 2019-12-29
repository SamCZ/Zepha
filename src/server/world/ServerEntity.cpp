//
// Created by aurailus on 2019-12-24.
//

#include "ServerEntity.h"

void ServerEntity::setPos(glm::vec3 position) {
    this->position = position;
}

glm::vec3 ServerEntity::getPos() {
    return position;
}

void ServerEntity::setVisualOffset(glm::vec3 vs) {
    this->visualOffset = vs;
}

glm::vec3 ServerEntity::getVisualOffset(){
    return visualOffset;
}

void ServerEntity::setAngle(float angle) {
    this->angle = angle;
}

float ServerEntity::getAngle() {
    return angle;
}

void ServerEntity::setScale(float scale) {
    this->scale = scale;
}

float ServerEntity::getScale() {
    return scale;
}

void ServerEntity::setAppearance(std::string appearance, std::string arg1, std::string arg2) {
    this->appearance = appearance;
    this->arg1 = arg1;
    this->arg2 = arg2;
}
