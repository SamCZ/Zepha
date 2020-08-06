//
// Created by aurailus on 10/06/19.
//

#include "ServerDefinitionAtlas.h"

#include "item/BlockDef.h"

ServerDefinitionAtlas::ServerDefinitionAtlas() {
    //Invalid Node
    BlockModel invalidModel = BlockModel::createCube({}, {}, {});
    BlockDef* invalid = new BlockDef();
    invalid->identifier = "invalid";
    invalid->name = "INVALID";
    invalid->maxStackSize = 64;
    invalid->model = invalidModel;
    invalid->farModel = invalidModel;
    invalid->sBoxes = {{{0, 0, 0}, {1, 1, 1}}};
    invalid->cBoxes = {{{0, 0, 0}, {1, 1, 1}}};
    registerDef(invalid);

    //Air Node
    BlockModel nullModel {};
    BlockDef* air = new BlockDef();
    air->identifier = "air";
    air->index = 1;
    air->name = "Air";
    air->maxStackSize = 64;
    air->model = nullModel;
    air->farModel = nullModel;
    air->solid = false;
    air->culls = false;
    registerDef(air);
}

void ServerDefinitionAtlas::registerDef(ItemDef* def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}