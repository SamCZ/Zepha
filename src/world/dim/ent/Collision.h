//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <glm/vec3.hpp>

#include "util/CovariantPtr.h"

class Subgame;
class Dimension;
class SelectionBox;

class Collision {
public:
    static bool isOnGround(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3 pos, glm::vec3 vel);
    static void moveCollide(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3& pos, glm::vec3& vel, float stepUpAmount = 0);
    static bool collidesAt(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3& pos, float stepUpMax = 0);
};
