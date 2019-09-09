//
// Created by aurailus on 30/12/18.
//

#include "Ray.h"

Ray::Ray(glm::vec3 pos, float yaw, float pitch) {
    this->start = pos;
    this->pos = pos;

    float yawR = glm::radians(yaw + 90);
    float pitchR = glm::radians(pitch);

    this->dir = glm::normalize(glm::vec3(
        std::sin(yawR) * std::cos(pitchR),
        std::sin(pitchR),
       -std::cos(yawR) * std::cos(pitchR)
    ));
}

Ray::Ray(Player* player) {
    this->start = player->getPos();
    this->start.y += Player::EYE_HEIGHT;
    this->pos = start;

    float yawR = glm::radians(player->getYaw() + 90);
    float pitchR = glm::radians(player->getPitch());

    this->dir = glm::normalize(glm::vec3(
            std::sin(yawR) * std::cos(pitchR),
            std::sin(pitchR),
           -std::cos(yawR) * std::cos(pitchR)
    ));
}

void Ray::step(float scale) {
    pos += (scale * dir);
}

glm::vec3* Ray::getEnd() {
    return &pos;
}

float Ray::getLength() {
    return glm::distance(start, pos);
}
