//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <string>
#include "block/BlockDef.h"
#include "ItemDef.h"

class DefinitionAtlas {
public:
    virtual void registerDef(ItemDef* def) = 0;

    ItemDef& fromId(unsigned int index);
    ItemDef& fromStr(const std::string& identifier);

    //Specializations that throw an error upon an invalid type.
    BlockDef& blockFromId(unsigned int index);
    BlockDef& blockFromStr(const std::string& identifier);

    inline BlockDef& craftItemFromId(unsigned int index);
    inline BlockDef& craftItemFromStr(const std::string& identifier);

    unsigned int size();

    const static unsigned int INVALID = 0;
    const static unsigned int AIR = 1;

    ~DefinitionAtlas();
protected:
    std::vector<ItemDef*> defs;
    std::unordered_map<std::string, unsigned int> defTable;
};
