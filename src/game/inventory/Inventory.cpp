//
// Created by aurailus on 2019-12-17.
//

#include "Inventory.h"

void Inventory::sendDirtyLists() {
    for (auto& list : lists) {
        if (list.second->dirty) {
            list.second->sendAll();
            list.second->dirty = false;
        }
    }
}

void Inventory::createList(std::string name, unsigned short length, unsigned short width) {
    lists.emplace(name, std::make_shared<ServerInventoryList>(defs, clients, this->name, name, length, width));
}

std::shared_ptr<ServerInventoryList> Inventory::operator[](std::string name) {
    if (lists.count(name)) return lists[name];
    else return nullptr;
}

void Inventory::removeList(std::string name) {
    lists.erase(name);
}

void Inventory::setDefaultList(const std::string &name) {
    defaultList = name;
}

std::string Inventory::getDefaultList() {
    return defaultList;
}
