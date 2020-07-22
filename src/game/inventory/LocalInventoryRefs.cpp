//
// Created by aurailus on 2020-02-24.
//

#include <iostream>

#include "LocalInventoryRefs.h"

#include "LocalInventory.h"
#include "LocalInventoryList.h"
#include "../../net/PacketView.h"
#include "../../def/LocalDefinitionAtlas.h"
#include "../../net/client/ClientNetworkInterpreter.h"

LocalInventoryRefs::LocalInventoryRefs(LocalDefinitionAtlas& defs, ClientNetworkInterpreter& net) : defs(defs) {
    namespace ph = std::placeholders;

    this->watchFn = std::bind(&ClientNetworkInterpreter::invWatch, &net, ph::_1, ph::_2);
    this->unWatchFn = std::bind(&ClientNetworkInterpreter::invUnwatch, &net, ph::_1, ph::_2);
    this->primaryCallback = std::bind(&ClientNetworkInterpreter::invInteractPrimary, &net, ph::_1, ph::_2, ph::_3);
    this->secondaryCallback = std::bind(&ClientNetworkInterpreter::invInteractSecondary, &net, ph::_1, ph::_2, ph::_3);

    inventories.insert({"current_player", std::make_shared<LocalInventory>(defs, "current_player", primaryCallback, secondaryCallback)});
    inventories["current_player"]->createList("cursor", 1, 1, true);
}

void LocalInventoryRefs::init() {
    watch("current_player", "cursor");
}

void LocalInventoryRefs::update(double delta, ClientNetworkInterpreter& net) {
    time += delta;

    for (auto mIt = inventories.begin(); mIt != inventories.end();) {
        if (mIt->second->pruneLists(net, time)) mIt = inventories.erase(mIt);
        else mIt++;
    }
}

void LocalInventoryRefs::packetReceived(std::unique_ptr<PacketView> p) {
    std::string source = p->d.read<std::string>();
    std::string list = p->d.read<std::string>();
    if (strncmp(source.data(), "player:", 7) == 0) source = "current_player";

    if (!inventories.count(source)) return;
    if (!inventories[source]->operator[](list)) return;

    unsigned int size = p->d.read<unsigned int>();
    unsigned int width = p->d.read<unsigned int>();

    std::vector<ItemStack> stacks {};
    stacks.reserve(size);

    while (!p->d.atEnd()) {
        unsigned short count = p->d.read<unsigned short>();
        unsigned int id = p->d.read<unsigned int>();
        stacks.push_back({id, count});
    }

    inventories[source]->operator[](list)->setData(size, width, stacks);
}

void LocalInventoryRefs::watch(const std::string &inv, const std::string &list, bool persistant) {
    if (!inventories.count(inv)) inventories.insert({inv, {}});
    if ((*inventories[inv])[list] == nullptr) {
        inventories[inv]->createList(list, 0, 0, persistant);
        watchFn(inv, list);
    }
    else (*inventories[inv]).setPersistant(list, persistant);
}

void LocalInventoryRefs::unWatch(const std::string &inv, const std::string &list) {
    if (inventories.count(inv) && (*inventories[inv])[list] != nullptr) (*inventories[inv]).removeList(list);
}

std::shared_ptr<LocalInventory> LocalInventoryRefs::getInv(const std::string& inv) {
    return inventories[inv];
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getList(const std::string& inv, const std::string& list) {
    watch(inv, list);
    return inventories[inv]->operator[](list);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getHandList() {
    return handList == "" ? nullptr : (*inventories["current_player"])[handList];
}

void LocalInventoryRefs::setHandList(const std::string &list) {
    if (list == handList) return;
    if (handList != "") unWatch("current_player", handList);
    handList = list;
    if (handList != "") watch("current_player", handList, true);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getWieldList() {
    return wieldList == "" ? nullptr : (*inventories["current_player"])[wieldList];
}

void LocalInventoryRefs::setWieldList(const std::string &list){
    if (list == wieldList) return;
    if (wieldList != "") unWatch("current_player", wieldList);
    wieldList = list;
    if (wieldList != "") watch("current_player", wieldList, true);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getCursorList() {
    return inventories["current_player"]->operator[]("cursor");
}
