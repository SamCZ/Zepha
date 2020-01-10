//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include "../../../world/Dimension.h"

class World {
public:
    explicit World() = default;

    virtual void update(double delta) = 0;

    virtual unsigned int getBlock(glm::ivec3 pos) = 0;
    virtual void setBlock(glm::ivec3 pos, unsigned int block) = 0;
};
