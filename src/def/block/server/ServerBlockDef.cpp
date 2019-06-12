//
// Created by aurailus on 10/06/19.
//

#include "ServerBlockDef.h"

ServerBlockDef::ServerBlockDef(std::string identifier, int index, ServerBlockModel model, bool solid, SelectionBox selectionBox) :
        BlockDef(std::move(identifier), index, model.culls, solid, selectionBox),
        model(std::move(model)) {}

ServerBlockModel& ServerBlockDef::getModel() {
    return model;
}