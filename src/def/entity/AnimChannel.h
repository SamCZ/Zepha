//
// Created by aurailus on 26/08/19.
//

#pragma once

#include <string>
#include <assimp/quaternion.h>
#include <glm/vec3.hpp>
#include <vector>

class AnimChannel {
public:
    AnimChannel() = default;
    AnimChannel(unsigned int index, const std::string& name);

    int index = -1;
    std::string name = "";

    std::vector<std::pair<double, glm::vec3>> positionKeys {};
    std::vector<std::pair<double, glm::vec3>> scaleKeys {};
    std::vector<std::pair<double, aiQuaternion>> rotationKeys {};
};
