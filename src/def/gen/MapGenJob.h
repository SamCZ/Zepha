//
// Created by aurailus on 13/02/19.
//

#ifndef ZEUS_MAPGENJOB_H
#define ZEUS_MAPGENJOB_H

#include <vector>
#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

struct MapGenJob {

    std::vector<int> blocks;
    std::vector<float> density;
    std::vector<int> depth;
    std::vector<float> depthFloat;

    glm::vec3 pos {};

    explicit MapGenJob(glm::vec3 pos) {

        this->pos = pos;

        blocks = std::vector<int>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        density = std::vector<float>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        depth = std::vector<int>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        depthFloat = std::vector<float>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
    }
};

#endif //ZEUS_MAPGENJOB_H
