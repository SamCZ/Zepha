//
// Created by aurailus on 24/08/19.
//

#ifndef ZEUS_ENTITYVERTEX_H
#define ZEUS_ENTITYVERTEX_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct EntityVertex {
    glm::vec3 position;
    glm::vec4 colorData;
    glm::vec3 colorBlend;
    float useTex;
    glm::vec3 normal;
};

#define STRIDE_OFFSET_ENTITY(m) sizeof(struct EntityVertex), (void *)offsetof(struct EntityVertex, m)

#endif //ZEUS_ENTITYVERTEX_H
