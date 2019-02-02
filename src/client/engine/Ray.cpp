//
// Created by aurailus on 30/12/18.
//

#include "Ray.h"

Ray::Ray(glm::vec3 position, float yaw, float pitch) {
    this->start = position;
    this->position = position;
    this->yaw = glm::radians(yaw + 90);
    this->pitch = glm::radians(pitch);
}

Ray::Ray(Player* player) {
    this->start = *player->getPos();
    this->position = start;
    this->yaw = glm::radians(player->getYaw() + 90);
    this->pitch = glm::radians(player->getPitch());
}

void Ray::step(float scale) {
    position.x += scale * std::sin(yaw) * std::cos(pitch);
    position.y += scale * std::sin(pitch);
    position.z += scale * -std::cos(yaw) * std::cos(pitch);
}

glm::vec3* Ray::getEnd() {
    return &position;
}

float Ray::getLength() {
    return glm::distance(start, position);
}

