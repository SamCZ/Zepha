//
// Created by aurailus on 2019-10-29.
//

#include "LocalInventoryList.h"

LocalInventoryList::LocalInventoryList(DefinitionAtlas& defs, const std::string& invName,
        const std::string& listName, unsigned short size, unsigned short width) :
    InventoryList(defs, invName, listName, size, width) {}

void LocalInventoryList::setData(unsigned int size, unsigned int width, std::vector<ItemStack> stacks) {
    this->width = width;
    this->itemstacks = stacks;
    manipulated();
}

void LocalInventoryList::setGuiCallback(std::function<void()> cb) {
    this->guiCallback = cb;
}

void LocalInventoryList::manipulated(){
    if (guiCallback != nullptr) guiCallback();
}

