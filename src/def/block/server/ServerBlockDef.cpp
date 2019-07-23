//
// Created by aurailus on 10/06/19.
//

#include "ServerBlockDef.h"

ServerBlockDef::ServerBlockDef(std::string identifier, int index, const ServerBlockModel& model, bool solid, SelectionBox selectionBox) :
        BlockDef {index, std::move(identifier), model.culls, solid, selectionBox},
        model(model) {}