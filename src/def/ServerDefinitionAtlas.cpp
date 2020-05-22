//
// Created by aurailus on 10/06/19.
//

#include "ServerDefinitionAtlas.h"

ServerDefinitionAtlas::ServerDefinitionAtlas() {

    //Invalid Node
    BlockModel invalidModel = BlockModel::createCube({}, {}, {});
    BlockDef* invalid = new BlockDef("invalid", "Invalid (you broke the game!)", 1, invalidModel, invalidModel, true, {}, false, {{}}, {{}}, INVALID);
    registerDef(invalid);

    //Air Node
    BlockModel nullModel {};
    BlockDef* air = new BlockDef("air", "Air (you broke the game!)", 1, nullModel, nullModel, false, {}, true, {}, {}, AIR);
    registerDef(air);
}

void ServerDefinitionAtlas::registerDef(ItemDef* def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}