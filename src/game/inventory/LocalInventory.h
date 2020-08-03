//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <set>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Inventory.h"

#include "LocalInventoryList.h"
#include "../../def/LocalSubgame.h"

class DefinitionAtlas;
class ClientNetworkInterpreter;

class LocalInventory : public Inventory {
public:
    LocalInventory(SubgamePtr game, const std::string& name, ClientNetworkInterpreter& net) :
        Inventory(game, name), net(net) {}

    virtual InventoryListPtr getList(const std::string& name) override;

    virtual void createList(const std::string& name, unsigned short length, unsigned short width) override;

    void setPersistant(const std::string& list, bool persistant);
    bool pruneLists(double time);

private:
    ClientNetworkInterpreter& net;
};
