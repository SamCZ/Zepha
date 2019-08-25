//
// Created by aurailus on 02/03/19.
//

#pragma once


#include <glm/vec3.hpp>
#include <glm/glm.hpp>

class FrustumPlane {
public:
    glm::vec3 normal, point;
    float d;

    FrustumPlane() = default;

    void setPoints(glm::vec3 &v1, glm::vec3 &v2, glm::vec3 &v3);

    float distance(glm::vec3 &p);
};

