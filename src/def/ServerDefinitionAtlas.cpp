//
// Created by aurailus on 10/06/19.
//

#include "ServerDefinitionAtlas.h"

ServerDefinitionAtlas::ServerDefinitionAtlas() {

    //Invalid Node
    BlockModel invalidModel = BlockModel::createCube({});
    BlockDef* invalid = new BlockDef("invalid", INVALID, "Invalid (you broke the game!)", invalidModel, true, {{}}, {{}});
    registerDef(invalid);

    //Air Node
    BlockModel nullModel {};
    BlockDef* air = new BlockDef("air", AIR, "Air", nullModel, false, {}, {});
    registerDef(air);
}

void ServerDefinitionAtlas::registerDef(ItemDef* def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}