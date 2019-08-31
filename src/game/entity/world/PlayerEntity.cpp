//
// Created by aurailus on 08/02/19.
//

#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(glm::vec3 pos, int peer_id, std::shared_ptr<AtlasRef> playerFrontTex, std::shared_ptr<AtlasRef> playerBackTex, std::shared_ptr<AtlasRef> shadowTex) :
    peer_id(peer_id),
    playerFrontTex(std::move(playerFrontTex)),
    playerBackTex(std::move(playerBackTex)),
    shadowTex(std::move(shadowTex)) {

    this->model = std::make_shared<Model>();

    auto fUV = this->playerFrontTex->uv;
    auto bUV = this->playerBackTex->uv;
    auto sUV = this->shadowTex->uv;

    std::vector<EntityVertex> vertices {
        //Front
        {{0.001, 2, -0.5}, {fUV.x, fUV.y, 0, 0}, {1, 1, 1}, true, {1, 0, 0}},
        {{0.001, 0, -0.5}, {fUV.x, fUV.w, 0, 0}, {1, 1, 1}, true, {1, 0, 0}},
        {{0.001, 0,  0.5}, {fUV.z, fUV.w, 0, 0}, {1, 1, 1}, true, {1, 0, 0}},
        {{0.001, 2,  0.5}, {fUV.z, fUV.y, 0, 0}, {1, 1, 1}, true, {1, 0, 0}},
        //Back
        {{-0.001, 2, -0.5}, {bUV.x, bUV.y, 0, 0}, {1, 1, 1}, true, {-1, 0, 0}},
        {{-0.001, 0, -0.5}, {bUV.x, bUV.w, 0, 0}, {1, 1, 1}, true, {-1, 0, 0}},
        {{-0.001, 0,  0.5}, {bUV.z, bUV.w, 0, 0}, {1, 1, 1}, true, {-1, 0, 0}},
        {{-0.001, 2,  0.5}, {bUV.z, bUV.y, 0, 0}, {1, 1, 1}, true, {-1, 0, 0}},
        //Shadow
        {{-0.5, 0, -0.5}, {sUV.x, sUV.y, 0, 0}, {1, 1, 1}, true, {0, 1, 0}},
        {{-0.5, 0,  0.5}, {sUV.x, sUV.w, 0, 0}, {1, 1, 1}, true, {0, 1, 0}},
        {{ 0.5, 0,  0.5}, {sUV.z, sUV.w, 0, 0}, {1, 1, 1}, true, {0, 1, 0}},
        {{ 0.5, 0, -0.5}, {sUV.z, sUV.y, 0, 0}, {1, 1, 1}, true, {0, 1, 0}},
    };
    std::vector<unsigned int> indices {
        0, 1, 2, 2, 3, 0,
        0, 2, 1, 2, 0, 3,

        4, 5, 6, 6, 7, 4,
        4, 6, 5, 6, 4, 7,

        8, 9, 10, 10, 11, 8,
        8, 10, 9, 10, 8, 11,
    };

    uptr<EntityMesh> mesh = std::make_unique<EntityMesh>();
    mesh->create(vertices, indices);
    this->model->fromMesh(std::move(mesh));
    this->setPos(pos);
}