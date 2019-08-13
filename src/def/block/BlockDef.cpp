//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"


BlockDef::BlockDef(const std::string &identifier, const BlockModel &model, bool solid, SelectionBox selectionBox) :
    BlockDef(identifier, 0, model, solid, selectionBox) {}

BlockDef::BlockDef(const std::string& identifier, unsigned int index, const BlockModel& model, bool solid, SelectionBox selectionBox) :
    ItemDef {identifier, index, ItemDef::Type::BLOCK},
    model(model),
    culls(model.culls),
    solid(solid),
    selectionBox(selectionBox) {}
