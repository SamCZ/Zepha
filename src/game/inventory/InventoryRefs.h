//
// Created by aurailus on 2020-07-29.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "../../util/CovariantPtr.h"

class Subgame;
class Inventory;

class InventoryRefs {
public:
    InventoryRefs(const InventoryRefs& o) = delete;
    explicit InventoryRefs(SubgamePtr game);

    virtual bool hasInventory(const std::string& inv);
    virtual InventoryPtr createInventory(const std::string& inv) = 0;
    virtual InventoryPtr getInventory(const std::string& inv);

protected:
    SubgamePtr game;

    std::unordered_map<std::string, std::shared_ptr<Inventory>> inventories {};
};
