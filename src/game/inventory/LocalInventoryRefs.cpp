//
// Created by aurailus on 2020-02-24.
//

#include "LocalInventoryRefs.h"

LocalInventoryRefs::LocalInventoryRefs(LocalDefinitionAtlas& defs, ClientNetworkInterpreter& net) :
    defs(defs),
    net(net) {

    inventories.insert({"current_player", std::make_shared<LocalInventory>(defs, "current_player")});
    inventories["current_player"]->createList("hand", 1, 1, true);
}

void LocalInventoryRefs::update(double delta) {
    time += delta;

    for (auto mIt = inventories.begin(); mIt != inventories.end();) {
        if (mIt->second->pruneLists(net, time)) mIt = inventories.erase(mIt);
        else mIt++;
    }
}

std::shared_ptr<LocalInventory> LocalInventoryRefs::getInv(const std::string& inv) {
    return inventories[inv];
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getList(const std::string& inv, const std::string& list) {
    if (!inventories.count(inv)) inventories.insert({inv, {}});
    if (inventories[inv]->operator[](list) == nullptr) {
        inventories[inv]->createList(list, 1, 1);
        net.watchInv(inv, list);
    }
    return inventories[inv]->operator[](list);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getHand() {
    return inventories["current_player"]->operator[]("hand");
}