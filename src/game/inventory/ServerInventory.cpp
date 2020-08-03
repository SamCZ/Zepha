//
// Created by aurailus on 2019-12-17.
//

#include "ServerInventory.h"

#include "ServerInventoryList.h"

void ServerInventory::createList(const std::string &name, unsigned short length, unsigned short width) {
    lists.emplace(name, std::make_shared<ServerInventoryList>(game, clients, name, this->name, length, width));
}

//void ServerInventory::sendDirtyLists() {
//    for (auto& list : lists) {
//        if (list.second->dirty) {
//            list.second->sendAll();
//            list.second->dirty = false;
//        }
//    }
//}