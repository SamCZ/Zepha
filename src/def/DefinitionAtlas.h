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

    ItemDef& fromId(unsigned int index) const;
    ItemDef& fromStr(const std::string& identifier) const;

    //Specializations that throw an error upon an invalid usertype.
    BlockDef& blockFromId(unsigned int index) const;
    BlockDef& blockFromStr(const std::string& identifier) const;
    CraftItemDef& craftItemFromId(unsigned int index) const;
    CraftItemDef& craftItemFromStr(const std::string& identifier) const;

    unsigned int size();

    const static unsigned int INVALID = 0;
    const static unsigned int AIR = 1;

    ~DefinitionAtlas();
protected:
    std::vector<ItemDef*> defs;
    std::unordered_map<std::string, unsigned int> defTable;
};
