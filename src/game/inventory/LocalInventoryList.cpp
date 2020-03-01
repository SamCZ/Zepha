//
// Created by aurailus on 2019-10-29.
//

#include "LocalInventoryList.h"

LocalInventoryList::LocalInventoryList(DefinitionAtlas& defs, const std::string& invName,
        const std::string& listName, unsigned short size, unsigned short width,
        std::function<void(unsigned short)> primaryCallback, std::function<void(unsigned short)> secondaryCallback) :
    InventoryList(defs, invName, listName, size, width),

    primaryCallback(primaryCallback),
    secondaryCallback(secondaryCallback) {}

void LocalInventoryList::primaryInteract(InventoryList &hand, unsigned short ind) {
    InventoryList::primaryInteract(hand, ind);
    primaryCallback(ind);
}

void LocalInventoryList::secondaryInteract(InventoryList &hand, unsigned short ind) {
    InventoryList::secondaryInteract(hand, ind);
    secondaryCallback(ind);
}

void LocalInventoryList::setData(unsigned int size, unsigned int width, std::vector<ItemStack> stacks) {
    this->width = width;
    this->itemstacks = stacks;
    manipulated();
}

void LocalInventoryList::setGuiCallback(std::function<void()> cb) {
    this->guiCallback = cb;
}

void LocalInventoryList::manipulated() {
    if (guiCallback != nullptr) guiCallback();
}