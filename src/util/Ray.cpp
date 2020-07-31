//
// Created by aurailus on 30/12/18.
//

#include <glm/glm.hpp>

#include "Ray.h"

#include "../game/scene/world/LocalPlayer.h"

Ray::Ray(glm::vec3 pos, float yawDeg, float pitchDeg) :
    pos(pos),
    start(pos) {

    float yaw = glm::radians(yawDeg + 90);
    float pitch = glm::radians(pitchDeg);

    this->dir = glm::normalize(glm::vec3(
        std::sin(yaw) * std::cos(pitch), std::sin(pitch),
       -std::cos(yaw) * std::cos(pitch) ));
}

Ray::Ray(Player& player) : pos(start),
    start(player.getPos() + player.getLookOffset()) {

    float yaw = glm::radians(player.getYaw() + 90);
    float pitch = glm::radians(player.getPitch());

    dir = glm::normalize(glm::vec3(
            std::sin(yaw) * std::cos(pitch), std::sin(pitch),
           -std::cos(yaw) * std::cos(pitch) ));
}

void Ray::step(float scale) {
    pos += scale * dir;
}

glm::vec3 Ray::getEnd() {
    return pos;
}

float Ray::getLength() {
    return glm::distance(start, pos);
}
