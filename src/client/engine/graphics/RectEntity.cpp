//
// Created by aurailus on 10/02/19.
//

#include "RectEntity.h"

RectEntity::RectEntity(glm::vec4 color) {
    createColored(color, color, color, color);
}

RectEntity::RectEntity(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    createColored(tl, tr, bl, br);
}

RectEntity::RectEntity(Texture *texture) {
    createTextured(texture);
}

void RectEntity::createColored(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    auto vertices = new std::vector<float> {
            0, 0, 0, 0, tl.w, tl.x, tl.y, tl.z,
            0, 1, 0, 0, bl.w, bl.x, bl.y, bl.z,
            1, 1, 0, 1, br.w, br.x, br.y, br.z,
            1, 0, 0, 1, tr.w, tr.x, tr.y, tr.z,
    };
    auto indices = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(vertices, indices);

    create(m);

    delete vertices;
    delete indices;
}


void RectEntity::createTextured(Texture *texture) {
    auto vertices = new std::vector<float> {
            0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 0,
            1, 1, 0, 1, 1, 0, 0, 0,
            1, 0, 0, 1, 0, 0, 0, 0,
    };
    auto indices = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(vertices, indices);

    create(m, texture);

    delete vertices;
    delete indices;
}


