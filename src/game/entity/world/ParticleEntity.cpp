//
// Created by aurailus on 15/05/19.
//

#include <cmath>
#include <random>
#include "ParticleEntity.h"

ParticleEntity::ParticleEntity(glm::vec3 pos, BlockDef &block) {
    setPos(pos + glm::vec3(.5,.3,.5));

//    setPos(getPos() + glm::vec3(
//            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2) - 1) / 3.f,
//            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2) - 1) / 3.f,
//            (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2) - 1) / 3.f
//    ));

    std::set<std::shared_ptr<AtlasRef>>& textureRefs = block.model.textureRefs;
    auto it = textureRefs.cbegin();
    advance(it, rand() % textureRefs.size());

    auto uv = (*it)->uv;

    auto spanX = uv.z - uv.x;
    auto spanY = uv.w - uv.y;

    float offX = spanX / 16 * (rand() % 12);
    float offY = spanX / 16 * (rand() % 12);

    uv -= glm::vec4(0, 0, spanX * 0.75, spanY * 0.75);
    uv += glm::vec4(offX, offY, offX, offY);

    std::vector<EntityVertex> vertices {
        {{-0.05, -0.05, 0}, {uv.x, uv.w, 0, 0}, {1, 1, 1}, true, {0, 0, 1}, {}, {}},
        {{-0.05,  0.05, 0}, {uv.x, uv.y, 0, 0}, {1, 1, 1}, true, {0, 0, 1}, {}, {}},
        {{ 0.05,  0.05, 0}, {uv.z, uv.y, 0, 0}, {1, 1, 1}, true, {0, 0, 1}, {}, {}},
        {{ 0.05, -0.05, 0}, {uv.z, uv.w, 0, 0}, {1, 1, 1}, true, {0, 0, 1}, {}, {}},
    };
    std::vector<unsigned int> indices {
        0, 1, 2, 2, 3, 0,
        0, 2, 1, 2, 0, 3,
    };

    auto dir = glm::radians(static_cast<float>(rand() % 360));
    float xDir = sinf(dir);
    float zDir = cosf(dir);

    float horiScale = 0.5f + (rand() % 100 / 100.f);

    velocity.y = 6.f;
    velocity.x = xDir * 2.f * horiScale;
    velocity.z = zDir * 2.f * horiScale;

    setPos(getPos() + glm::vec3(xDir, 0, zDir) / 4.f);

    uptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
    mesh->create(vertices, indices);
    this->model->fromMesh(std::move(mesh));
    this->setScale(0.75f + (rand() % 10 / 20.f));
}

void ParticleEntity::update(double delta, glm::vec3 player) {
    time += delta;

    setPos(getPos() + velocity * glm::vec3(static_cast<float>(delta)));

    double angle = -glm::degrees(std::atan2(player.z - position.z, player.x - position.x)) + 90.f;
    setRotateY(static_cast<float>(angle));

    velocity.y -= 32.f  * delta;
}

void ParticleEntity::draw(Renderer &renderer) {
    Entity::draw(renderer);
}
