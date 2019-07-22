//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_BLOCKDEF_H
#define ZEUS_BLOCKDEF_H


#include <string>
#include <unordered_map>
#include <sol2/sol.hpp>
#include "SelectionBox.h"
#include "../../api/Callback.h"

class BlockDef {
public:
    int index;
    std::string identifier;

    bool culls = false;
    bool solid = false;

    SelectionBox selectionBox;

    std::unordered_map<Callback, sol::function, Util::EnumClassHash> callbacks {};
};


#endif //ZEUS_BLOCKDEF_H
