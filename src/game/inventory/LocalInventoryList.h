//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "InventoryList.h"

class LocalInventoryList : public InventoryList {
public:
    LocalInventoryList(DefinitionAtlas& defs, const std::string& invName,
        const std::string& listName, unsigned short size, unsigned short width,
        std::function<void(unsigned short)> primaryCallback, std::function<void(unsigned short)> secondaryCallback);

    void primaryInteract(InventoryList& hand, unsigned short ind) override;
    void secondaryInteract(InventoryList& hand, unsigned short ind) override;

    void setData(unsigned int size, unsigned int width, std::vector<ItemStack> stacks);
    void setGuiCallback(std::function<void()> cb);

private:
    void manipulated() override;
    std::function<void()> guiCallback = nullptr;
    std::function<void(unsigned short)> primaryCallback = nullptr;
    std::function<void(unsigned short)> secondaryCallback = nullptr;
};
