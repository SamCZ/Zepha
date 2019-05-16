#include <cmath>

//
// Created by aurailus on 08/04/19.
//

#ifndef ZEUS_SELECTIONBOX_H
#define ZEUS_SELECTIONBOX_H

#include <iostream>
#include <vec3.hpp>
#include <glm.hpp>
#include "../../../util/Util.h"

class SelectionBox {
public:
    enum Face {
        NONE,
        TOP, BOTTOM, FRONT, BACK, LEFT, RIGHT
    };

    SelectionBox() {
        set({0, 0, 0}, {1, 1, 1});
    }

    SelectionBox(glm::vec3 a, glm::vec3 b) {
        set(a, b);
    }

    SelectionBox& operator=(const SelectionBox &s) {
        a = s.a;
        b = s.b;
    }

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

    Face intersects(glm::vec3 vec, glm::vec3 blockOffset) {
        const float THRESH = 0.02f;

        vec -= blockOffset; //Normalize Vector Position

        if (fabs(vec.y - b.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z) return TOP;
        if (fabs(vec.y - a.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z) return BOTTOM;
        if (fabs(vec.z - a.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y) return FRONT;
        if (fabs(vec.z - b.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y) return BACK;
        if (fabs(vec.x - b.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y) return LEFT;
        if (fabs(vec.x - a.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y) return RIGHT;

        return NONE;
    }

    static glm::vec3 faceToOffset(SelectionBox::Face f) {
        static const glm::vec3 positionOffsets[6] {
            {0, 1, 0},
            {0, -1, 0},
            {0, 0, -1},
            {0, 0, 1},
            {1, 0, 0},
            {-1, 0, 0}
        };

        return positionOffsets[f - 1];
    }

    glm::vec3 a {}, b {};
};


#endif //ZEUS_SELECTIONBOX_H
