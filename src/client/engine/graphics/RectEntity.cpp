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
            0, 0, 0, 0, tl.x, tl.y, tl.z, tl.w, 0, 0, 0,
            0, 1, 0, 0, bl.x, bl.y, bl.z, bl.w, 0, 0, 0,
            1, 1, 0, 0, br.x, br.y, br.z, br.w, 0, 0, 0,
            1, 0, 0, 0, tr.x, tr.y, tr.z, tr.w, 0, 0, 0,
    };
    auto indices = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(vertices, indices);

    setMesh(m);

    delete vertices;
    delete indices;
}


void RectEntity::createTextured(Texture *texture) {
    auto vertices = new std::vector<float> {
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0,
            1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0,
            1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    };
    auto indices = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(vertices, indices);

    setMesh(m, texture);

    delete vertices;
    delete indices;
}


