//
// Created by aurailus on 10/06/19.
//

#include "ServerDefinitionAtlas.h"

ServerDefinitionAtlas::ServerDefinitionAtlas() {
    BlockModel nullModel;
    nullModel.visible = false,
    nullModel.culls = false;

    //Invalid Node
    BlockDef* invalid = new BlockDef("invalid", INVALID, nullModel, true, {{0, 0, 0}, {1, 1, 1}});
    registerDef(invalid);

    //Air Node
    BlockDef* air = new BlockDef("air", AIR, nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    registerDef(air);
}

void ServerDefinitionAtlas::registerDef(ItemDef* def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}