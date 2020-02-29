//
// Created by aurailus on 2020-02-24.
//

#include "LocalInventory.h"

void LocalInventory::createList(std::string name, unsigned short length, unsigned short width, bool maintain) {
    lists.insert({name, {(maintain ? -1 : 0), std::make_shared<LocalInventoryList>(defs, this->name, name, length, width)}});
}

std::shared_ptr<LocalInventoryList> LocalInventory::operator[](std::string name) {
    if (lists.count(name)) return lists[name].second;
    else return nullptr;
}

bool LocalInventory::pruneLists(ClientNetworkInterpreter &net, double time) {
    for (auto lIt = lists.begin(); lIt != lists.end();) {
        if (lIt->second.first != -1) {
            // Start the timeout for Inventories that aren't being used.
            if (lIt->second.first == 0 && lIt->second.second.use_count() == 1) lIt->second.first = time + 15;
            // Remove the timeout for Inventories that are being used.
            else if (lIt->second.first != 0 && lIt->second.second.use_count() > 1) lIt->second.first = 0;
            // Delete InventoryLists that have passed their timeout.
            else if (lIt->second.first != 0 && lIt->second.first <= time) {
                net.unwatchInv(name, lIt->first);
                lIt = lists.erase(lIt);
            }
            else lIt++;
        }
        else lIt++;
    }

    return lists.size() == 0;
}