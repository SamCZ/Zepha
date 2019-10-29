//
// Created by aurailus on 2019-10-28.
//

#pragma once

#include "../scene/world/LocalWorld.h"

class Collidable {
public:
    Collidable(LocalWorld& world, const SelectionBox& collisionBox);

    void moveCollide();
    bool isOnGround();

    glm::vec3 pos {};
    glm::vec3 vel {};
protected:
    LocalWorld& world;
    SelectionBox collisionBox {};
private:
    bool collidesAt(const glm::vec3& pos);
};
