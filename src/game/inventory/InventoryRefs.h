//
// Created by aurailus on 2020-07-29.
//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>

class Subgame;
class Inventory;

class InventoryRefs {
public:
    InventoryRefs(const InventoryRefs& o) = delete;
    explicit InventoryRefs(Subgame& game);

    virtual bool hasInventory(const std::string& inv);
    virtual Inventory& createInventory(const std::string& inv) = 0;
    virtual Inventory& getInventory(const std::string& inv);

protected:
    Subgame& game;

    std::unordered_map<std::string, std::shared_ptr<Inventory>> inventories {};
};
