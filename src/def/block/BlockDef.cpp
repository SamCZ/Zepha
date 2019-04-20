//
// Created by aurailus on 02/12/18.
//

#include "BlockDef.h"

BlockModel& BlockDef::getModel() {
    return model;
}

BlockDef::BlockDef(std::string identifier, BlockModel model, bool solid, SelectionBox selectionBox) {
    this->identifier = std::move(identifier);
    this->model = std::move(model);
    this->culls = this->model.culls;
    this->solid = solid;
    this->selectionBox = selectionBox;
}

bool BlockDef::isSolid() {
    return solid;
}

std::string BlockDef::getIdentifier() {
    return identifier;
}

bool BlockDef::isCulling() {
    return culls;
}

SelectionBox BlockDef::getSelectionBox() {
    return selectionBox;
}
