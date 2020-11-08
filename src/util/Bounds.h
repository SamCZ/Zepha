//
// Created by auri on 2020-11-07.
//

#pragma once

#include <glm/vec3.hpp>

class Bounds {
public:
    Bounds() = default;
    Bounds(glm::vec3 a, glm::vec3 b);

    glm::vec3 getA();
    glm::vec3 getB();
    void setA(glm::vec3 vecA);
    void setB(glm::vec3 vecB);

    bool intersects(glm::vec3 test);

private:
    glm::vec3 a, b;
};
