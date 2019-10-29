//
// Created by aurailus on 2019-10-28.
//

#include "Collidable.h"

Collidable::Collidable(LocalWorld &world, const SelectionBox& collisionBox) :
    world(world),
    collisionBox(collisionBox) {}

void Collidable::moveCollide() {
    const static double increment = 0.05;

    double moved = 0;
    for (int i = 0; i < fabs(vel.y) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.y) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.y += move * (vel.y < 0 ? -1 : 1);

        if (!collidesAt(newPos))
            pos = newPos;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.x) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.x) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.x += move * (vel.x < 0 ? -1 : 1);

        if (!collidesAt(newPos))
            pos = newPos;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.z) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.z) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.z += move * (vel.z < 0 ? -1 : 1);

        if (!collidesAt(newPos)) pos = newPos;
    }
}

bool Collidable::collidesAt(const glm::vec3 &pos) {
    float xOffset = collisionBox.a.x;
    while (true) {
        float yOffset = collisionBox.a.y;
        while (true) {
            float zOffset = collisionBox.a.z;
            while (true) {
                if (world.solidAt(pos + glm::vec3 {xOffset, yOffset, zOffset})) return true;

                if (zOffset == collisionBox.b.z) break;
                zOffset = std::min(std::floor(zOffset + 1), collisionBox.b.z);
            }
            if (yOffset == collisionBox.b.y) break;
            yOffset = std::min(std::floor(yOffset + 1), collisionBox.b.y);
        }
        if (xOffset == collisionBox.b.x) break;
        xOffset = std::min(std::floor(xOffset + 1), collisionBox.b.x);
    }

    return false;
}

bool Collidable::isOnGround() {
    return collidesAt(glm::vec3(pos.x, pos.y - 0.05f, pos.z)) && vel.y <= 0;
}
