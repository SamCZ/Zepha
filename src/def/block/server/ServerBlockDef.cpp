//
// Created by aurailus on 10/06/19.
//

#include "ServerBlockDef.h"

ServerBlockDef::ServerBlockDef(const std::string& identifier, int index, const ServerBlockModel& model, bool solid, SelectionBox selectionBox) :
        BlockDef {index, identifier, model.culls, solid, selectionBox},
        model(model) {}