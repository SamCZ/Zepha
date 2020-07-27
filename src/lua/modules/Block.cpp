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
}

std::string Api::Module::Block::getBlock(glm::ivec3 pos) {
    return game.getDefs().fromId(world.getBlock(pos)).identifier;
}

void Api::Module::Block::setBlock(glm::ivec3 pos, const std::string &identifier) {
    world.setBlock(pos, game.getDefs().fromStr(identifier).index);
}
