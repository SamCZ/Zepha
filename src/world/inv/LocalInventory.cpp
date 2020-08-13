//
// Created by aurailus on 2020-02-24.
//

#include "LocalInventory.h"

#include "LocalInventoryList.cpp"
#include "client/conn/ClientNetworkInterpreter.h"

InventoryListPtr LocalInventory::getList(const std::string &name) {
    if (!hasList(name)) createList(name, 0, 0);
    return Inventory::getList(name);
}

void LocalInventory::createList(const std::string &name, unsigned short length, unsigned short width) {
    lists.emplace(name, std::make_shared<LocalInventoryList>(game, name, this->name, length, width, net));
    net.invWatch(this->name, name);
}

void LocalInventory::setPersistant(const std::string &list, bool persistant) {
    if (!lists.count(name)) return;
    std::static_pointer_cast<LocalInventoryList>(lists[list])->persistant = persistant;
}

bool LocalInventory::pruneLists(double time) {
    for (auto lIt = lists.begin(); lIt != lists.end();) {
        auto list = std::static_pointer_cast<LocalInventoryList>(lIt->second);
        if (!list->persistant) {
            // Start the timeout for Inventories that aren't being used.
            if (list->decayTime == 0 && list.use_count() == 2) list->decayTime = time + 15;
            // Remove the timeout for Inventories that are being used.
            else if (list->decayTime != 0 && list.use_count() > 2) list->decayTime = 0;
            // Delete InventoryLists that have passed their timeout.
            else if (list->decayTime != 0 && list->decayTime <= time) {
                net.invUnwatch(name, lIt->first);
                lIt = lists.erase(lIt);
            }
            else lIt++;
        }
        else lIt++;
    }

    return lists.size() == 0;
}
