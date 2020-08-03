//
// Created by aurailus on 2020-02-24.
//

#include <iostream>

#include "LocalInventoryRefs.h"

#include "LocalInventoryList.h"
#include "../../net/PacketView.h"
#include "../../def/LocalDefinitionAtlas.h"
#include "../../net/client/ClientNetworkInterpreter.h"

LocalInventoryRefs::LocalInventoryRefs(SubgamePtr game, ClientNetworkInterpreter& net) : InventoryRefs(game), net(net) {}

void LocalInventoryRefs::init() {
    createInventory("current_player");
    getInventory("current_player")->createList("cursor", 1, 1);
    watch("current_player", "cursor");
}

void LocalInventoryRefs::update(double delta, ClientNetworkInterpreter& net) {
    time += delta;

    for (auto mIt = inventories.begin(); mIt != inventories.end();) {
        if (std::static_pointer_cast<LocalInventory>(mIt->second)->pruneLists(time)) mIt = inventories.erase(mIt);
        else mIt++;
    }
}

InventoryPtr LocalInventoryRefs::createInventory(const std::string &inv) {
    if (!inventories.count(inv)) inventories.emplace(inv, std::make_shared<LocalInventory>(game, inv, net));
    return inventories[inv];
}

void LocalInventoryRefs::packetReceived(std::unique_ptr<PacketView> p) {
    std::string source = p->d.read<std::string>();
    std::string list = p->d.read<std::string>();
    if (strncmp(source.data(), "player:", 7) == 0) source = "current_player";

    if (!inventories.count(source)) return;
    if (!inventories[source]->hasList(list)) return;

    unsigned int size = p->d.read<unsigned int>();
    unsigned int width = p->d.read<unsigned int>();

    std::vector<ItemStack> stacks {};
    stacks.reserve(size);

    while (!p->d.atEnd()) {
        unsigned short count = p->d.read<unsigned short>();
        unsigned int id = p->d.read<unsigned int>();
        stacks.push_back({id, count});
    }

    inventories[source]->getList(list).l()->setData(size, width, stacks);
}

void LocalInventoryRefs::watch(const std::string &inv, const std::string &list, bool persistant) {
    if (!inventories.count(inv)) inventories.insert({inv, {}});
    if (!inventories[inv]->hasList(list)) {
        inventories[inv]->createList(list, 0, 0);
        net.invWatch(inv, list);
    }
    std::static_pointer_cast<LocalInventory>(inventories[inv])->setPersistant(list, persistant);
}

void LocalInventoryRefs::unWatch(const std::string &inv, const std::string &list) {
    if (inventories.count(inv) && inventories[inv]->hasList(list)) inventories[inv]->removeList(list);
    net.invUnwatch(inv, list);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getHandList() {
    return handList;
}

void LocalInventoryRefs::setHandList(const std::string &list) {
    if (handList && list == handList->getName()) return;
    if (handList) unWatch("current_player", handList->getName());
    handList = getInventory("current_player")->getList(list).l();
    getInventory("current_player").l()->setPersistant(list, true);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getWieldList() {
    return wieldList;
}

void LocalInventoryRefs::setWieldList(const std::string &list) {
    if (wieldList && list == wieldList->getName()) return;
    if (wieldList) unWatch("current_player", wieldList->getName());
    wieldList = getInventory("current_player")->getList(list).l();
    getInventory("current_player").l()->setPersistant(list, true);
}

std::shared_ptr<LocalInventoryList> LocalInventoryRefs::getCursorList() {
    return inventories["current_player"]->getList("cursor").l();
}