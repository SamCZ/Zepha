//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockDef.h"

LocalBlockDef::LocalBlockDef(std::string identifier, const LocalBlockModel& model, bool solid, SelectionBox selectionBox) :
    BlockDef {-1, std::move(identifier), model.culls, solid, selectionBox},
    model(model) {}