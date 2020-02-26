//
// Created by aurailus on 2020-02-24.
//

#pragma once


#include "LocalInventoryList.h"
#include "../../def/DefinitionAtlas.h"
#include "../scene/net/ClientNetworkInterpreter.h"

class LocalInventory {
public:
    LocalInventory(DefinitionAtlas& defs, const std::string& name) : defs(defs), name(name) {};

    void createList(std::string name, unsigned short length, unsigned short width, bool maintain = false);
    std::shared_ptr<LocalInventoryList> operator[](std::string name);

    bool pruneLists(ClientNetworkInterpreter& net, double time);

    DefinitionAtlas& defs;
private:
    std::string name;
    std::unordered_map<std::string, std::pair<double, std::shared_ptr<LocalInventoryList>>> lists;
};
