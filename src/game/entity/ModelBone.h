//
// Created by aurailus on 25/08/19.
//

#pragma once

#include <string>
#include <vector>
#include <glm/mat4x4.hpp>

class ModelBone {
public:
    ModelBone() = default;
    ModelBone(unsigned int index, int parent, const std::string& name);

    std::string name {};
    unsigned int index = 0;
    int parent = 0;

    glm::mat4 offsetMatrix {};

    std::vector<ModelBone*> children;
};
