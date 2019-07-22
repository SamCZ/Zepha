//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKDEF_H
#define ZEUS_SERVERBLOCKDEF_H

#include <string>
#include "../SelectionBox.h"
#include "ServerBlockModel.h"
#include "../BlockDef.h"

class ServerBlockDef : public BlockDef {
public:
    ServerBlockDef() = default;
    ServerBlockDef(std::string identifier, int index, ServerBlockModel model, bool solid, SelectionBox selectionBox);

    ServerBlockModel model;
};


#endif //ZEUS_SERVERBLOCKDEF_H
