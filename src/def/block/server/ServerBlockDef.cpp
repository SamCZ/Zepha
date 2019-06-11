//
// Created by aurailus on 10/06/19.
//

#include "ServerBlockDef.h"

ServerBlockModel& ServerBlockDef::getModel() {
    return model;
}

ServerBlockDef::ServerBlockDef(std::string identifier, ServerBlockModel model, bool solid, SelectionBox selectionBox) :
        identifier(std::move(identifier)),
        solid(solid),
        culls(model.culls),
        model(std::move(model)),
        selectionBox(selectionBox) {}

bool ServerBlockDef::isSolid() {
    return solid;
}

std::string ServerBlockDef::getIdentifier() {
    return identifier;
}

bool ServerBlockDef::isCulling() {
    return culls;
}

SelectionBox ServerBlockDef::getSelectionBox() {
    return selectionBox;
}