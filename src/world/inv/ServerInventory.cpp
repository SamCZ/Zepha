//
// Created by aurailus on 2019-12-17.
//

#include "ServerInventory.h"

#include "ServerInventoryList.h"

void ServerInventory::createList(const std::string &name, unsigned short length, unsigned short width) {
    lists.emplace(name, std::make_shared<ServerInventoryList>(game, clients, name, this->name, length, width));
}

void ServerInventory::sendDirtyLists() {
    for (auto& l : lists) {
        auto list = std::static_pointer_cast<ServerInventoryList>(l.second);
        if (list->dirty) list->sendToAll();
    }
}