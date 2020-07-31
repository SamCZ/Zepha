//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <glm/vec3.hpp>

class Subgame;
class Dimension;
class SelectionBox;

class Collision {
public:
    static bool isOnGround(Subgame& game, Dimension& dim, SelectionBox& collision, glm::vec3 pos, glm::vec3 vel);
    static void moveCollide(Subgame& game, Dimension& dimension, SelectionBox& collision, glm::vec3& pos, glm::vec3& vel, float stepUpAmount = 0);
    static bool collidesAt(Subgame& game, Dimension& dimension, SelectionBox& collision, glm::vec3& pos, float stepUpMax = 0);
};
