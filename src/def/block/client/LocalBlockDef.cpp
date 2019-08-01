//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockDef.h"

LocalBlockDef::LocalBlockDef(const std::string& identifier, const LocalBlockModel& model, bool solid, SelectionBox selectionBox) :
    BlockDef {0, identifier, model.culls, solid, selectionBox},
    model(model) {}