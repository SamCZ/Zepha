//
// Created by aurailus on 2020-07-25.
//

#include "Block.h"

#include "../Lua.h"
#include "../../def/ItemDef.h"
#include "../../def/Subgame.h"
#include "../../def/DefinitionAtlas.h"
#include "../../game/scene/world/World.h"

void Api::Module::Block::bind() {
    core.set_function("get_block", Util::bind_this(this, &Block::getBlock));
    core.set_function("set_block", Util::bind_this(this, &Block::setBlock));
    core.set_function("remove_block", [&](glm::ivec3 pos) { setBlock(pos, "air"); });

    core.set_function("block_damage_get", Util::bind_this(this, &Block::damageGet));
    core.set_function("block_damage_set", Util::bind_this(this, &Block::damageSet));
    core.set_function("block_damage_add", Util::bind_this(this, &Block::damageAdd));
}

std::string Api::Module::Block::getBlock(glm::ivec3 pos) {
    return game.getDefs().fromId(world.getBlock(pos)).identifier;
}

void Api::Module::Block::setBlock(glm::ivec3 pos, const std::string &identifier) {
    world.setBlock(pos, game.getDefs().fromStr(identifier).index);
}

double Api::Module::Block::damageGet(glm::ivec3 pos) {
    return world.getBlockDamage(pos);
}

double Api::Module::Block::damageSet(glm::ivec3 pos, double damage) {
    return world.setBlockDamage(pos, damage);
}

double Api::Module::Block::damageAdd(glm::ivec3 pos, double damage) {
    return world.setBlockDamage(pos, world.getBlockDamage(pos) + damage);
}
