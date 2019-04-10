//
// Created by aurailus on 08/02/19.
//

#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(glm::vec3 pos, int peer_id) {
    this->peer_id = peer_id;

    this->crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair_clean.png");
    crosshairTexture->load();

    auto vertices = new std::vector<float> {
            -0.5, -0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            -0.5,  0.5, 0, 1, 0, 1, 0, 0, 0, 0, 0,
            0.5,  0.5, 0, 1, 1, 1, 0, 0, 0, 0, 0,
            0.5, -0.5, 0, 1, 1, 0, 0, 0, 0, 0, 0,

            0, -0.5, -0.5, 1, 0, 0, 0, 0, 0, 0, 0,
            0,  0.5, -0.5, 1, 0, 1, 0, 0, 0, 0, 0,
            0,  0.5,  0.5, 1, 1, 1, 0, 0, 0, 0, 0,
            0, -0.5,  0.5, 1, 1, 0, 0, 0, 0, 0, 0,

            -0.5, 0, -0.5, 1, 0, 0, 0, 0, 0, 0, 0,
            0.5, 0, -0.5, 1, 0, 1, 0, 0, 0, 0, 0,
            0.5, 0,  0.5, 1, 1, 1, 0, 0, 0, 0, 0,
            -0.5, 0,  0.5, 1, 1, 0, 0, 0, 0, 0, 0,
    };
    auto indices = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0,
            0, 2, 1, 2, 0, 3,
            4, 5, 6, 6, 7, 4,
            4, 6, 5, 6, 4, 7,
            8, 9, 10, 10, 11, 8,
            8, 10, 9, 10, 8, 11
    };

    auto m = new Mesh();
    m->create(vertices, indices);

    setMesh(m, crosshairTexture);

    delete vertices;
    delete indices;

    this->setPos(pos);
    this->setScale(0.5);
}

PlayerEntity::~PlayerEntity() {
    crosshairTexture->clear();
    delete crosshairTexture;
}