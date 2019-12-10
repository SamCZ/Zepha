//
// Created by aurailus on 2019-10-28.
//

#pragma once

#include "../scene/world/LocalWorld.h"

class Collidable {
public:
    Collidable(LocalWorld& world, LocalDefs& defs, const SelectionBox& collisionBox);

    void moveCollide(float stepUpAmount = 0);
    bool isOnGround();

    glm::vec3 pos {};
    glm::vec3 vel {};
protected:
    LocalWorld& world;
    LocalDefs& defs;
    SelectionBox collisionBox {};
private:
    bool collidesAt(glm::vec3& pos, float stepUpMax = 0);
};
