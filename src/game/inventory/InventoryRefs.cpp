//
// Created by aurailus on 2020-02-26.
//

#include "InventoryRefs.h"

#include "InventoryList.h"

InventoryRefs::InventoryRefs(ServerDefinitionAtlas &defs, ClientList* clients) :
    defs(defs), clients(clients) {}

void InventoryRefs::update() {
    for (auto& inv : inventories) {
        inv.second->sendDirtyLists();
    }
}

std::shared_ptr<Inventory> InventoryRefs::createInv(const std::string &inv) {
    inventories.emplace(inv, std::make_shared<Inventory>(defs, clients, inv));
    return inventories[inv];
}

std::shared_ptr<Inventory> InventoryRefs::getInv(const std::string &inv) {
    return inventories[inv];
}

std::shared_ptr<InventoryList> InventoryRefs::getList(const std::string &inv, const std::string &list) {
    if (!inventories.count(inv)) return nullptr;
    return inventories[inv]->operator[](list);
}

bool InventoryRefs::addWatcher(const std::string &inv, const std::string &list, unsigned int cid) {
    if (!inventories.count(inv)) return false;
    if (!inventories[inv]->operator[](list)) return false;

    inventories[inv]->operator[](list)->addWatcher(cid);
    return true;
}

bool InventoryRefs::removeWatcher(const std::string &inv, const std::string &list, unsigned int cid) {
    if (!inventories.count(inv)) return false;
    if (!inventories[inv]->operator[](list)) return false;

    inventories[inv]->operator[](list)->removeWatcher(cid);
    return true;
}
