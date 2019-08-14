//
// Created by aurailus on 10/06/19.
//

#include "ServerDefinitionAtlas.h"

ServerDefinitionAtlas::ServerDefinitionAtlas() {
    BlockModel nullModel;
    nullModel.visible = false,
    nullModel.culls = false;

    //Invalid Node
    BlockDef* invalid = new BlockDef("invalid", INVALID, "Invalid (you broke the game!)", nullModel, true, {});
    registerDef(invalid);

    //Air Node
    BlockDef* air = new BlockDef("air", AIR, "Air", nullModel, false, {});
    registerDef(air);
}

void ServerDefinitionAtlas::registerDef(ItemDef* def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}