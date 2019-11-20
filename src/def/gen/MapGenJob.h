//
// Created by aurailus on 13/02/19.
//

#pragma once

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

struct MapGenJob {
    std::array<float, 4096> density {};
    std::array<float, 4096> depth {};
};
