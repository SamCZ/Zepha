#include <cmath>

//
// Created by aurailus on 15/05/19.
//

#include "ParticleEntity.h"

ParticleEntity::ParticleEntity(glm::vec3 pos, BlockDef &block) {
    this->position = pos + glm::vec3(0.5);

    float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2) - 1;
    float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2) - 1;
    float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2) - 1;

    this->position += glm::vec3(x / 3.f, y / 3.f, z / 3.f);

    auto uv = block.getModel().topFaces[0].texture->uv;
    auto spanX = uv.z - uv.x;
    auto spanY = uv.w - uv.y;

    //Divisor of 16 to get 6. Delete magic number later
    uv.x += spanX / 2.666;
    uv.y += spanY / 2.666;
    uv.z -= spanX / 2.666;
    uv.w -= spanY / 2.666;

    std::vector<Vertex> vertices {
        {{-0.05, -0.05, 0}, 1, {uv.x, uv.y, 0, 0}, {}},
        {{-0.05,  0.05, 0}, 1, {uv.x, uv.w, 0, 0}, {}},
        {{ 0.05,  0.05, 0}, 1, {uv.z, uv.w, 0, 0}, {}},
        {{ 0.05, -0.05, 0}, 1, {uv.z, uv.y, 0, 0}, {}},
    };
    std::vector<unsigned int> indices {
        0, 1, 2, 2, 3, 0,
        0, 2, 1, 2, 0, 3,
    };

    auto dir = glm::radians(static_cast<float>(random() % 360));
    float xDir = sinf(dir);
    float zDir = cosf(dir);

    velocity.y = 6.f;
    velocity.x = xDir * 2.f;
    velocity.z = zDir * 2.f;

    pos += glm::vec3(xDir, 0, zDir);

    auto m = new Mesh();
    m->create(vertices, indices);
    this->setMesh(m);
}

void ParticleEntity::update(double delta, glm::vec3 player) {
    position += velocity * glm::vec3(static_cast<float>(delta));

    double angle = -glm::degrees(std::atan2(player.z - position.z, player.x - position.x)) + 90.f;
    setAngle(static_cast<float>(angle));

//    velocity.x *= 0.95f / delta;
//    velocity.z *= 0.95f / delta;
    velocity.y -= 32.f * delta;
}

void ParticleEntity::draw(Renderer &renderer) {
    Entity::draw(renderer);
}
