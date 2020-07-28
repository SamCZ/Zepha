//
// Created by aurailus on 2020-01-09.
//

#include "World.h"

#include "../../../def/Subgame.h"
#include "../../../def/item/BlockDef.h"
#include "../../../def/DefinitionAtlas.h"

World::World(Subgame &game) : game(game) {}

double World::getBlockDamage(glm::ivec3 pos) const {
    return blockDamages.count(pos) ? blockDamages.at(pos).curr : 0;
}

double World::setBlockDamage(glm::ivec3 pos, double damage) {
    if (blockDamages.count(pos)) blockDamages[pos].curr = damage;
    else blockDamages.insert({pos, Damage { damage, static_cast<double>(game.getDefs().blockFromId(getBlock(pos)).health)}});
    return getBlockDamage(pos);
}

void World::updateBlockDamages() {
    for (auto it = blockDamages.begin(); it != blockDamages.end(); ) {
        if (it->second.curr > it->second.max) {
            setBlock(it->first, DefinitionAtlas::AIR);
            it = blockDamages.erase(it);
        }
        else it++;
    }
}
