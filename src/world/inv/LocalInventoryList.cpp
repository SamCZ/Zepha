//
// Created by aurailus on 2019-10-29.
//

#include "LocalInventoryList.h"

#include "client/conn/ClientNetworkInterpreter.h"

LocalInventoryList::LocalInventoryList(SubgamePtr game, const std::string& name,
    const std::string& invName, unsigned short size, unsigned short width,
    ClientNetworkInterpreter& net) :
    InventoryList(game, name, invName, size, width),
    net(net) {}

void LocalInventoryList::interact(InventoryListPtr hand, bool primary, unsigned short ind) {
    InventoryList::interact(hand, primary, ind);
    net.invInteract(invName, name, primary, ind);
}

void LocalInventoryList::setData(unsigned int size, unsigned int width, std::vector<ItemStack> items) {
    this->width = width;
    this->items = items;
    manipulated();
}

void LocalInventoryList::addGuiCallback(std::shared_ptr<std::function<void()>> cb) {
    guiCallbacks.push_back(cb);
}

void LocalInventoryList::removeGuiCallback(std::shared_ptr<std::function<void()>> cb) {
    for (auto it = guiCallbacks.begin(); it != guiCallbacks.end();) {
        if (cb == (*it)) {
            guiCallbacks.erase(it);
            return;
        }
        it++;
    }

    throw std::runtime_error("Failed to remove callback.");
}

void LocalInventoryList::manipulated() {
    for (auto& cb : guiCallbacks) (*cb)();
}