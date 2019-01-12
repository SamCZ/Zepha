#include <utility>

//
// Created by aurailus on 02/12/18.
//

#include "BlockDef.h"

BlockModel *BlockDef::getModel() {
    return model;
}

BlockDef::BlockDef(std::string identifier, BlockModel *model, bool solid) {
    this->identifier = std::move(identifier);
    this->model = model;
    this->solid = solid;
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
