//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKDEF_H
#define GLPROJECT_BLOCKDEF_H

#include <string>
#include "LocalBlockModel.h"
#include "../SelectionBox.h"
#include "../BlockDef.h"

class LocalBlockDef : public BlockDef {
public:
    LocalBlockDef() = default;
    LocalBlockDef(std::string identifier, LocalBlockModel model, bool solid, SelectionBox selectionBox);

    LocalBlockModel model;
};


#endif //GLPROJECT_BLOCKDEF_H
