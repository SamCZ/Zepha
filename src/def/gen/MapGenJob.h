//
// Created by aurailus on 13/02/19.
//

#ifndef ZEUS_MAPGENJOB_H
#define ZEUS_MAPGENJOB_H

#include <vec3.hpp>
#include <vector>

struct MapGenJob {

    std::vector<int> blocks;
    std::vector<float> density;
    std::vector<int> depth;

    glm::vec3 pos {};

    explicit MapGenJob(glm::vec3 pos) {

        this->pos = pos;

        blocks = std::vector<int>(4096);
        density = std::vector<float>(4096);
        depth = std::vector<int>(4096);
    }
};

#endif //ZEUS_MAPGENJOB_H
