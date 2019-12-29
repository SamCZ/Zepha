//
// Created by aurailus on 2019-12-24.
//

#pragma once

#include <glm/vec3.hpp>
#include <string>

class ServerEntity {
public:
    ServerEntity() = default;

    void setPos(glm::vec3 position);
    glm::vec3 getPos();

    void setVisualOffset(glm::vec3 vs);
    glm::vec3 getVisualOffset();

    void setAngle(float angle);
    float getAngle();

    void setScale(float scale);
    float getScale();

    void setAppearance(std::string appearance, std::string arg1, std::string arg2);
protected:
    glm::vec3 position {};
    glm::vec3 visualOffset {};
    float angle = 0;
    float scale = 1;

    std::string appearance = {};
    std::string arg1 = {};
    std::string arg2 = {};
};

