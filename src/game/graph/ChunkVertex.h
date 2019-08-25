//
// Created by aurailus on 24/08/19.
//

#ifndef ZEUS_CHUNKVERTEX_H
#define ZEUS_CHUNKVERTEX_H

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    float     normal;
    float     shaderMod;
    glm::vec3 modValues;
};

#define STRIDE_OFFSET_CHUNK(m) sizeof(struct ChunkVertex), (void *)offsetof(struct ChunkVertex, m)

#endif //ZEUS_CHUNKVERTEX_H
