//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKDEF_H
#define GLPROJECT_BLOCKDEF_H

#include <string>
#include "LocalBlockModel.h"
#include "../SelectionBox.h"

class LocalBlockDef {
public:
    LocalBlockDef() = default;
    LocalBlockDef(std::string identifier, LocalBlockModel model, bool solid, SelectionBox selectionBox);

    bool         isCulling();
    bool         isSolid();
    LocalBlockModel&  getModel();
    std::string  getIdentifier();
    SelectionBox getSelectionBox();
private:
    bool culls = false;
    bool solid = false;
    SelectionBox selectionBox;
    LocalBlockModel   model;
    std::string  identifier;
};


#endif //GLPROJECT_BLOCKDEF_H
