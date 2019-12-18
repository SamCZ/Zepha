//
// Created by aurailus on 2019-12-17.
//

#include "Inventory.h"

void Inventory::createList(std::string name, unsigned short length, unsigned short width) {
    lists.emplace(name, std::make_shared<InventoryList>(defs, length, width));
}

std::shared_ptr<InventoryList> Inventory::operator[](std::string name) {
    if (lists.count(name)) return lists[name];
    else return nullptr;
}

void Inventory::removeList(std::string name) {
    lists.erase(name);
}
