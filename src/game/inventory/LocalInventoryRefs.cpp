//
// Created by aurailus on 2020-02-24.
//

#include "LocalInventoryRefs.h"

#include "../scene/net/ClientNetworkInterpreter.h"

LocalInventoryRefs::LocalInventoryRefs(LocalDefinitionAtlas& defs) :
    defs(defs) {

    inventories.insert({"current_player", std::make_shared<LocalInventory>(defs, "current_player")});
    inventories["current_player"]->createList("hand", 1, 1, true);
}

void LocalInventoryRefs::setWatchFunction(std::function<void(std::string, std::string)> watchFn) {
    this->watchFn = watchFn;
}

void LocalInventoryRefs::update(double delta, ClientNetworkInterpreter& net) {
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
        inventories[inv]->createList(list, 0, 0);
        watchFn(inv, list);
    }
    return inventories[inv]->operator[](list);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getHand() {
    return inventories["current_player"]->operator[]("hand");
}

void LocalInventoryRefs::packetReceived(std::unique_ptr<Packet> p) {
    Deserializer d(p->data);

    std::string source = d.read<std::string>();
    std::string list = d.read<std::string>();
    if (strncmp(source.data(), "player:", 7) == 0) source = "current_player";

    if (!inventories.count(source)) return;
    if (!inventories[source]->operator[](list)) return;

    unsigned int size = d.read<unsigned int>();
    unsigned int width = d.read<unsigned int>();

    std::vector<ItemStack> stacks {};
    stacks.reserve(size);

    while (!d.atEnd()) {
        unsigned short count = d.read<unsigned short>();
        unsigned int id = d.read<unsigned int>();
        stacks.push_back({id, count});
    }

    inventories[source]->operator[](list)->setData(size, width, stacks);
}