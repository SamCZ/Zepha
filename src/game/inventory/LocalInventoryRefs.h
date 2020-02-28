//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <string>
#include <unordered_map>

#include "LocalInventory.h"
#include "LocalInventoryList.h"
#include "../../def/LocalDefinitionAtlas.h"

class LocalInventoryRefs {
public:
    LocalInventoryRefs(LocalDefinitionAtlas& defs);

    void setWatchFunction(std::function<void(std::string, std::string)> watchFn);

    void update(double delta, ClientNetworkInterpreter& net);
    void packetReceived(std::unique_ptr<Packet> p);

    std::shared_ptr<LocalInventory> getInv(const std::string& inv);
    std::shared_ptr<LocalInventoryList> getList(const std::string& inv, const std::string& list);
    std::shared_ptr<LocalInventoryList> getHand();
private:
    std::unordered_map<std::string, std::shared_ptr<LocalInventory>> inventories {};

    std::function<void(std::string, std::string)> watchFn = nullptr;

    LocalDefinitionAtlas& defs;

    double time = 0;
};
