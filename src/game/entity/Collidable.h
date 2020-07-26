//
// Created by aurailus on 2019-10-28.
//

#pragma once

#include <glm/vec3.hpp>

#include "../../def/item/SelectionBox.h"

class LocalWorld;
class LocalSubgame;

class Collidable {
public:
    Collidable(LocalWorld& world, LocalSubgame& defs, const SelectionBox& collisionBox);

    void moveCollide(float stepUpAmount = 0);
    bool isOnGround();

    glm::vec3 pos {};
    glm::vec3 vel {};
protected:
    LocalWorld& world;
    LocalSubgame& game;
    SelectionBox collisionBox {};
private:
    bool collidesAt(glm::vec3& pos, float stepUpMax = 0);
};
