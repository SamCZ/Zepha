//
// Created by aurailus on 13/05/19.
//

#pragma once

#include "../util/Vec.h"

class Target {
public:
    enum class Type { ENTITY, BLOCK, NOTHING };

    Target() = default;
    Target(const Target &o) = default;
    Target(glm::ivec3 pos, EVec face);

    glm::ivec3 getAbovePos() const;

public:
    Type type = Target::Type::NOTHING;

    glm::vec3 pos {};
    EVec face = EVec::NONE;
};
