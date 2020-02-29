//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "InventoryList.h"

class LocalInventoryList : public InventoryList {
public:
    LocalInventoryList(DefinitionAtlas& defs, const std::string& invName,
            const std::string& listName, unsigned short size, unsigned short width);

    void setData(unsigned int size, unsigned int width, std::vector<ItemStack> stacks);
    void setGuiCallback(std::function<void()> cb);

private:
    void manipulated() override;
    std::function<void()> guiCallback = nullptr;
};
