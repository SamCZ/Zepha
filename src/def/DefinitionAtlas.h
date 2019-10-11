//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <string>
#include "item/BlockDef.h"
#include "ItemDef.h"
#include "item/CraftItemDef.h"

class DefinitionAtlas {
public:
    virtual void registerDef(ItemDef* def) = 0;

    ItemDef& fromId(unsigned int index);
    ItemDef& fromStr(const std::string& identifier);

    //Specializations that throw an error upon an invalid type.
    BlockDef& blockFromId(unsigned int index);
    BlockDef& blockFromStr(const std::string& identifier);
    CraftItemDef& craftItemFromId(unsigned int index);
    CraftItemDef& craftItemFromStr(const std::string& identifier);

    unsigned int size();

    const static unsigned int INVALID = 0;
    const static unsigned int AIR = 1;

    ~DefinitionAtlas();
protected:
    std::vector<ItemDef*> defs;
    std::unordered_map<std::string, unsigned int> defTable;
};
