//
// Created by aurailus on 08/04/19.
//

#ifndef ZEUS_SELECTIONBOX_H
#define ZEUS_SELECTIONBOX_H


#include <vec3.hpp>

class SelectionBox {
public:
    SelectionBox() {
        set({0, 0, 0}, {1, 1, 1});
    }

    SelectionBox(glm::vec3 a, glm::vec3 b) {
        set(a, b);
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

    glm::vec3 a {}, b {};
};


#endif //ZEUS_SELECTIONBOX_H
