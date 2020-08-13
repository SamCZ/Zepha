//
// Created by aurailus on 2020-07-30.
//

#include <algorithm>

#include "Collision.h"

#include "game/Subgame.h"
#include "world/dim/Dimension.h"
#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"
#include "game/def/mesh/SelectionBox.h"

bool Collision::isOnGround(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3 pos, glm::vec3 vel) {
    pos.y -= 0.05f;
    return collidesAt(game, dim, collision, pos) && vel.y <= 0;
}

void Collision::moveCollide(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3& pos, glm::vec3& vel, float stepUpAmount) {
    const static double increment = 0.05;

    double moved = 0;
    for (int i = 0; i < std::abs(vel.y) / increment; i++) {
        double move = std::max(std::min(increment, std::abs(vel.y) - moved), 0.);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.y += move * (vel.y < 0 ? -1 : 1);

        if (!collidesAt(game, dim, collision, newPos)) pos = newPos;
        else if (vel.y > 0) vel.y = 0;
    }

    moved = 0;
    for (int i = 0; i < std::abs(vel.x) / increment; i++) {
        double move = std::max(std::min(increment,std::abs(vel.x) - moved), 0.);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.x += move * (vel.x < 0 ? -1 : 1);

        if (!collidesAt(game, dim, collision, newPos, stepUpAmount)) pos = newPos;
        else vel.x = 0;
    }

    moved = 0;
    for (int i = 0; i < std::abs(vel.z) / increment; i++) {
        double move = std::max(std::min(increment, std::abs(vel.z) - moved), 0.);
        moved += increment;

        glm::vec3 newPos = pos;
        newPos.z += move * (vel.z < 0 ? -1 : 1);

        if (!collidesAt(game, dim, collision, newPos, stepUpAmount)) pos = newPos;
        else vel.z = 0;
    }
}

bool Collision::collidesAt(SubgamePtr game, DimensionPtr dim, SelectionBox& collision, glm::vec3& pos, float stepUpMax) {

    // Find the minimum vertical increase needed to step up
    float stepUpAmount = 0;
    if (stepUpMax > 0) {
        glm::vec3 offset {};

        offset.x = collision.a.x;
        while (true) {
            offset.y = collision.a.y;
            while (true) {
                offset.z = collision.a.z;
                while (true) {
                    glm::vec3 offsetPos = glm::floor(pos + offset);
                    auto &def = game->getDefs().blockFromId(dim->getBlock(offsetPos));

                    if (def.solid)
                        for (auto &cBox : def.cBoxes)
                            stepUpAmount = std::fmax(cBox.b.y + offsetPos.y - pos.y, stepUpAmount);

                    if (offset.z == collision.b.z) break;
                    offset.z = std::fmin(std::floor(offset.z + 1), collision.b.z);
                }
                if (offset.y == collision.a.y + stepUpMax + 0.025f) break; // Hack for precision errors
                offset.y = std::fmin(std::floor(offset.y + 1), collision.a.y + stepUpMax + 0.025f);
            }
            if (offset.x == collision.b.x) break;
            offset.x = std::fmin(std::floor(offset.x + 1), collision.b.x);
        }
    }

    // Step up if possible, or return false
    if (stepUpAmount > stepUpMax) return true;
    if (stepUpAmount > 0) pos.y += stepUpAmount + 0.025; // Hack for precision errors

    SelectionBox collidableBox = {collision.a + pos, collision.b + pos};
    glm::vec3 offset {};

    // Regular collision check
    offset.x = collision.a.x;
    while (true) {
        offset.y = collision.a.y;
        while (true) {
            offset.z = collision.a.z;
            while (true) {
                glm::vec3 offsetPos = glm::floor(pos + offset);
                auto& def = game->getDefs().blockFromId(dim->getBlock(offsetPos));

                if (def.solid) {
                    for (auto &cBox : def.cBoxes) {
                        SelectionBox blockBox = {cBox.a + offsetPos, cBox.b + offsetPos};

                        if ((blockBox.a.x <= collidableBox.b.x && blockBox.b.x >= collidableBox.a.x) &&
                            (blockBox.a.y <= collidableBox.b.y && blockBox.b.y >= collidableBox.a.y) &&
                            (blockBox.a.z <= collidableBox.b.z && blockBox.b.z >= collidableBox.a.z)) return true;
                    }
                }

                if (offset.z == collision.b.z) break;
                offset.z = (std::min)(std::floor(offset.z + 1), collision.b.z);
            }
            if (offset.y == collision.b.y) break;
            offset.y = (std::min)(std::floor(offset.y + 1), collision.b.y);
        }
        if (offset.x == collision.b.x) break;
        offset.x = (std::min)(std::floor(offset.x + 1), collision.b.x);
    }

    return false;
}