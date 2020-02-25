//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <map>
#include <string>
#include <memory>
#include "InventoryList.h"

class Inventory {
public:
    Inventory(DefinitionAtlas& defs) : defs(defs) {};

    void createList(std::string name, unsigned short length, unsigned short width);
    std::shared_ptr<InventoryList> operator[](std::string name);
    void removeList(std::string name);

    DefinitionAtlas& defs;
private:
    std::map<std::string, std::shared_ptr<InventoryList>> lists;
};
