//
// Created by aurailus on 02/12/18.
//

#include "LocalBlockDef.h"

LocalBlockModel& LocalBlockDef::getModel() {
    return model;
}

LocalBlockDef::LocalBlockDef(std::string identifier, LocalBlockModel model, bool solid, SelectionBox selectionBox) :
    identifier(std::move(identifier)),
    culls(model.culls),
    model(std::move(model)),
    solid(solid),
    selectionBox(selectionBox) {}

bool LocalBlockDef::isSolid() {
    return solid;
}

std::string LocalBlockDef::getIdentifier() {
    return identifier;
}

bool LocalBlockDef::isCulling() {
    return culls;
}

SelectionBox LocalBlockDef::getSelectionBox() {
    return selectionBox;
}