//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"


BlockDef::BlockDef(const std::string &identifier, const std::string& name, const BlockModel &model, bool solid, const std::vector<SelectionBox>& sBoxes) :
    BlockDef(identifier, 0, name, model, solid, sBoxes) {}

BlockDef::BlockDef(const std::string& identifier, unsigned int index, const std::string& name, const BlockModel& model, bool solid, const std::vector<SelectionBox>& sBoxes) :
    ItemDef {identifier, name, index, ItemDef::Type::BLOCK},
    model(model),
    culls(model.culls),
    solid(solid),
    sBoxes(sBoxes) {}
