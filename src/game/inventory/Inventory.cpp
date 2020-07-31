//
// Created by aurailus on 2020-07-29.
//

#include "Inventory.h"

#include "InventoryList.h"

Inventory::Inventory(Subgame &game, const std::string& name) : game(game), name(name) {}

bool Inventory::hasList(const std::string &name) {
    return lists.count(name);
}

void Inventory::removeList(const std::string &name) {
    lists.erase(name);
}

InventoryList& Inventory::getList(const std::string &name) {
    if (lists.count(name)) return *lists[name];
    throw std::runtime_error("List " + name + " doesn't exist in Inventory " + this->name + ".");
}

std::shared_ptr<InventoryList> Inventory::getListPtr(const std::string &name) {
    // A dirty hack to cause LocalInventory to create a temp list.
    try { getList(name); } catch (...) {}
    return lists[name];
}