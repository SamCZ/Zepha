//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <list>

#include "InventoryList.h"

class LocalInventoryList : public InventoryList {
public:
    LocalInventoryList(DefinitionAtlas& defs, const std::string& invName,
        const std::string& listName, unsigned short size, unsigned short width,
        std::function<void(unsigned short)> primaryCallback, std::function<void(unsigned short)> secondaryCallback);

    void primaryInteract(InventoryList& hand, unsigned short ind) override;
    void secondaryInteract(InventoryList& hand, unsigned short ind) override;

    void setData(unsigned int size, unsigned int width, std::vector<ItemStack> stacks);
    void addGuiCallback(std::shared_ptr<std::function<void()>> cb);
    void removeGuiCallback(std::shared_ptr<std::function<void()>> cb);

private:
    void manipulated() override;
    std::function<void(unsigned short)> primaryCallback = nullptr;
    std::function<void(unsigned short)> secondaryCallback = nullptr;

    std::list<std::shared_ptr<std::function<void()>>> guiCallbacks {};
};
