//
// Created by aurailus on 02/12/18.
//

#include "BlockDef.h"

BlockModel *BlockDef::getModel() {
    return model;
}

BlockDef::BlockDef(std::string identifier, BlockModel *model, bool solid, SelectionBox selectionBox) {
    this->identifier = std::move(identifier);
    this->model = model;
    this->culls = model->culls;
    this->solid = solid;
    this->selectionBox = selectionBox;
}

BlockDef::~BlockDef() {
    delete this->model;
}

bool BlockDef::isSolid() {
    return solid;
}

std::string BlockDef::getIdentifier() {
    return identifier;
}

bool BlockDef::isCulling(){
    return culls;
}

SelectionBox BlockDef::getSelectionBox() {
    return selectionBox;
}
