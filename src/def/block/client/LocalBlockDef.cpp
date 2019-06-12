//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockDef.h"

LocalBlockDef::LocalBlockDef(std::string identifier, int index, LocalBlockModel model, bool solid, SelectionBox selectionBox) :
    BlockDef(std::move(identifier), index, model.culls, solid, selectionBox),
    model(std::move(model)) {}

LocalBlockModel& LocalBlockDef::getModel() {
    return model;
}