//
// Created by aurailus on 2020-02-26.
//

#include "ServerInventoryRefs.h"

#include "ServerInventory.h"
#include "ServerInventoryList.h"

ServerInventoryRefs::ServerInventoryRefs(SubgamePtr game, ServerClients& clients) :
    InventoryRefs(game), clients(clients) {}

void ServerInventoryRefs::update() {
//    for (auto& inv : inventories)
//        std::static_pointer_cast<ServerInventory>(inv.second)->sendDirtyLists();
}

InventoryPtr ServerInventoryRefs::createInventory(const std::string &inv) {
    if (!inventories.count(inv)) inventories.emplace(inv, std::make_shared<ServerInventory>(game, inv, clients));
    return inventories[inv];
}

bool ServerInventoryRefs::addWatcher(const std::string &inv, const std::string &list, unsigned int id) {
    std::string invReal = inv == "current_player" ? "player:" + std::to_string(id) : inv;

    if (!hasInventory(invReal)) return false;
    auto inventory = getInventory(invReal);
    if (!inventory->hasList(list)) return false;
    inventory->getList(list).s()->addWatcher(id);

    return true;
}

bool ServerInventoryRefs::removeWatcher(const std::string &inv, const std::string &list, unsigned int id) {
    std::string invReal = inv == "current_player" ? "player:" + std::to_string(id) : inv;

    if (!hasInventory(invReal)) return false;
    auto inventory = getInventory(invReal);
    if (!inventory->hasList(list)) return false;
    inventory->getList(list).s()->removeWatcher(id);

    return true;
}

bool ServerInventoryRefs::interact(bool primary, const std::string &inv, const std::string &list, unsigned short ind, unsigned int id) {
    std::string playerInv = "player:" + std::to_string(id);
    if (!hasInventory(playerInv)) return false;
    auto playerInventory = getInventory(playerInv);

    if (!hasInventory(inv)) return false;
    auto inventory = getInventory(inv);
    if (!inventory->hasList(list)) return false;

    inventory->getList(list).s()->interact(playerInventory->getList("cursor"), primary, ind);
    return true;
}