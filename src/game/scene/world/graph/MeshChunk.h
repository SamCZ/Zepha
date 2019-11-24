//
// Created by aurailus on 15/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

#pragma once

#include <glm/vec3.hpp>
#include <list>
#include "ChunkRenderElem.h"
#include "../../../entity/Entity.h"
#include "../../../../util/Space.h"

class MeshChunk : public ChunkRenderElem, Drawable {
public:
    MeshChunk() = default;
    void create(std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices);
    void draw(Renderer& renderer) override;

    bool updateChunkUse(glm::vec3 chunk, bool used) override;

    void setPos(glm::vec3 pos);
    glm::vec3 getPos() override;
private:
    uptr<ChunkMesh> mesh = nullptr;
    glm::vec3 pos {};
};


#pragma clang diagnostic pop