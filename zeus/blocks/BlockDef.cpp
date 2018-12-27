#include <utility>

//
// Created by aurailus on 02/12/18.
//

#include "BlockDef.h"

BlockModel *BlockDef::getModel() {
    return model;
}

BlockDef::BlockDef(std::string identifier, BlockModel *model) {
    this->identifier = std::move(identifier);
    this->model = model;
}

BlockDef::~BlockDef() {
    delete this->model;
}

std::string BlockDef::getIdentifier() {
    return identifier;
}
