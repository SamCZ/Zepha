//
// Created by aurailus on 28/09/19.
//

#pragma once

#include <glm/vec3.hpp>

class Renderer;

struct ChunkRenderElem {
    virtual void draw(Renderer& renderer) = 0;
    virtual glm::vec3 getPos() = 0;
    // Used to determine if the RenderElem should be deleted.
    // Bool is if the RenderElem should be kept alive.
    // True = keep, False = remove
    virtual bool updateChunkUse(glm::vec3 chunk, bool used) = 0;
};
