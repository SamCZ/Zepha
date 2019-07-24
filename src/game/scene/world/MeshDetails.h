//
// Created by aurailus on 23/07/19.
//

#ifndef ZEUS_MESHDETAILS_H
#define ZEUS_MESHDETAILS_H

#include <vector>
#include <glm/vec3.hpp>
#include "../../graph/ChunkMesh.h"

struct MeshDetails {
    std::vector<ChunkVertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 pos {0, 0, 0};
};

#endif //ZEUS_MESHDETAILS_H
