//
// Created by aurailus on 2019-10-28.
//

#include "Collidable.h"

Collidable::Collidable(LocalWorld &world, ClientGame& defs, const SelectionBox& collisionBox) :
    world(world),
    defs(defs),
    collisionBox(collisionBox) {}

void Collidable::moveCollide(float stepUpAmount) {
    const static double increment = 0.05;

    double moved = 0;
    for (int i = 0; i < fabs(vel.y) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.y) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.y += move * (vel.y < 0 ? -1 : 1);

        if (!collidesAt(newPos)) pos = newPos;
        else if (vel.y > 0) vel.y = 0;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.x) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.x) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.x += move * (vel.x < 0 ? -1 : 1);

        if (!collidesAt(newPos, stepUpAmount)) pos = newPos;
        else vel.x = 0;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.z) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.z) - moved), 0);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.z += move * (vel.z < 0 ? -1 : 1);

        if (!collidesAt(newPos, stepUpAmount)) pos = newPos;
        else vel.z = 0;
    }
}

bool Collidable::collidesAt(glm::vec3& pos, float stepUpMax) {
    // Find the minimum vertical increase needed to step up
    float stepUpAmount = 0;
    if (stepUpMax > 0) {
        glm::vec3 offset {};

        offset.x = collisionBox.a.x;
        while (true) {
            offset.y = collisionBox.a.y;
            while (true) {
                offset.z = collisionBox.a.z;
                while (true) {
                    glm::vec3 offsetPos = glm::floor(pos + offset);
                    auto &def = defs.defs.blockFromId(world.getBlock(offsetPos));

                    if (def.solid)
                        for (auto &cBox : def.cBoxes)
                            stepUpAmount = (std::max)(cBox.b.y + offsetPos.y - pos.y, stepUpAmount);

                    if (offset.z == collisionBox.b.z) break;
                    offset.z = (std::min)(std::floor(offset.z + 1), collisionBox.b.z);
                }
                if (offset.y == collisionBox.a.y + stepUpMax + 0.025f) break; // Hack for precision errors
                offset.y = (std::min)(std::floor(offset.y + 1), collisionBox.a.y + stepUpMax + 0.025f);
            }
            if (offset.x == collisionBox.b.x) break;
            offset.x = (std::min)(std::floor(offset.x + 1), collisionBox.b.x);
        }
    }

    // Step up if possible, or return false
    if (stepUpAmount > stepUpMax) return true;
    if (stepUpAmount > 0) pos.y += stepUpAmount + 0.025; // Hack for precision errors

    SelectionBox collidableBox = {collisionBox.a + pos, collisionBox.b + pos};
    glm::vec3 offset {};

    // Regular collision check
    offset.x = collisionBox.a.x;
    while (true) {
        offset.y = collisionBox.a.y;
        while (true) {
            offset.z = collisionBox.a.z;
            while (true) {
                glm::vec3 offsetPos = glm::floor(pos + offset);
                auto& def = defs.defs.blockFromId(world.getBlock(offsetPos));

                if (def.solid) {
                    for (auto &cBox : def.cBoxes) {
                        SelectionBox blockBox = {cBox.a + offsetPos, cBox.b + offsetPos};

                        if ((blockBox.a.x <= collidableBox.b.x && blockBox.b.x >= collidableBox.a.x) &&
                            (blockBox.a.y <= collidableBox.b.y && blockBox.b.y >= collidableBox.a.y) &&
                            (blockBox.a.z <= collidableBox.b.z && blockBox.b.z >= collidableBox.a.z)) return true;
                    }
                }

                if (offset.z == collisionBox.b.z) break;
                offset.z = (std::min)(std::floor(offset.z + 1), collisionBox.b.z);
            }
            if (offset.y == collisionBox.b.y) break;
            offset.y = (std::min)(std::floor(offset.y + 1), collisionBox.b.y);
        }
        if (offset.x == collisionBox.b.x) break;
        offset.x = (std::min)(std::floor(offset.x + 1), collisionBox.b.x);
    }

    return false;
}

bool Collidable::isOnGround() {
    glm::vec3 test = {pos.x, pos.y - 0.05f, pos.z};
    return collidesAt(test) && vel.y <= 0;
}
