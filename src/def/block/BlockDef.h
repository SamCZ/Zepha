//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_BLOCKDEF_H
#define ZEUS_BLOCKDEF_H


#include <string>
#include "SelectionBox.h"

class BlockDef {
public:
    BlockDef() = default;
    BlockDef(std::string identifier, int index, bool culls, bool solid, SelectionBox selectionBox) :
        identifier(std::move(identifier)),
        index(index),
        culls(culls),
        solid(solid),
        selectionBox(selectionBox) {}

    std::string getIdentifier() { return identifier; };
    int getIndex() { return index; }

    bool isCulling() { return culls; };
    bool isSolid() { return solid; };

    SelectionBox getSelectionBox() { return selectionBox; };
protected:
    std::string identifier;
    int index;

    bool culls = false;
    bool solid = false;

    SelectionBox selectionBox;
};


#endif //ZEUS_BLOCKDEF_H
