//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <string>
#include <unordered_map>

#include "LocalInventory.h"
#include "LocalInventoryList.h"
#include "../net/ClientNetworkInterpreter.h"
#include "../../../def/LocalDefinitionAtlas.h"

class LocalInventoryRefs {
public:
    LocalInventoryRefs(LocalDefinitionAtlas& defs, ClientNetworkInterpreter& net);

    void update(double delta);

    std::shared_ptr<LocalInventory> getInv(const std::string& inv);
    std::shared_ptr<LocalInventoryList> getList(const std::string& inv, const std::string& list);
    std::shared_ptr<LocalInventoryList> getHand();
private:
    std::unordered_map<std::string, std::shared_ptr<LocalInventory>> inventories {};

    ClientNetworkInterpreter& net;
    LocalDefinitionAtlas& defs;

    double time = 0;
};
