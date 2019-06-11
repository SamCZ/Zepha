//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKDEF_H
#define ZEUS_SERVERBLOCKDEF_H

#include <string>
#include "../SelectionBox.h"
#include "ServerBlockModel.h"

class ServerBlockDef {
public:
    ServerBlockDef() = default;
    ServerBlockDef(std::string identifier, ServerBlockModel model, bool solid, SelectionBox selectionBox);

    std::string getIdentifier();

    bool isCulling();
    bool isSolid();

    ServerBlockModel& getModel();
    SelectionBox getSelectionBox();
private:
    std::string identifier;

    bool culls = false;
    bool solid = false;

    ServerBlockModel model;
    SelectionBox selectionBox;
};


#endif //ZEUS_SERVERBLOCKDEF_H
