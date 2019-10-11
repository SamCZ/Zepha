#include <cmath>

//
// Created by aurailus on 08/04/19.
//

#pragma once

#include <iostream>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../../util/Util.h"
#include "../../util/Dir.h"

class SelectionBox {
public:
    SelectionBox() = default;
    SelectionBox(glm::vec3 a, glm::vec3 b) : a(a), b(b) {};
    SelectionBox& operator=(const SelectionBox &s) = default;

    bool operator==(const SelectionBox &o) {
        return (a == o.a && b == o.b);
    }

    bool operator!=(const SelectionBox &o) {
        return (a != o.a || b != o.b);
    }

    void set(glm::vec3 a, glm::vec3 b) {
        this->a = a;
        this->b = b;
    }

    Dir intersects(glm::vec3 vec, glm::vec3 blockOffset) {
        const float THRESH = 0.02f;

        vec -= blockOffset; //Normalize Vector Position

        if (std::abs(vec.y - b.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z) return TOP;
        if (std::abs(vec.y - a.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z) return BOTTOM;
        if (std::abs(vec.z - a.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y) return FRONT;
        if (std::abs(vec.z - b.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y) return BACK;
        if (std::abs(vec.x - b.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y) return LEFT;
        if (std::abs(vec.x - a.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y) return RIGHT;

        return NONE;
    }

    static glm::vec3 faceToOffset(Dir f) {
        static const glm::vec3 positionOffsets[6] {
            { 1, 0, 0},
            {-1, 0, 0},
            { 0, 1, 0},
            { 0,-1, 0},
            { 0, 0,-1},
            { 0, 0, 1},
        };

        return positionOffsets[f];
    }

    glm::vec3 a {}, b {1, 1, 1};
};

