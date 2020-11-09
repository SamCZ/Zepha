//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <list>

#include "InventoryList.h"

class ClientNetworkInterpreter;

class LocalInventoryList : public InventoryList {
public:
    LocalInventoryList(SubgamePtr game, const std::string& name,
        const std::string& invName, unsigned short size, unsigned short width,
        ClientNetworkInterpreter& net);

    void interact(InventoryListPtr hand, bool primary, unsigned short ind) override;

    void setData(unsigned int newWidth, const std::vector<ItemStack>& newItems);
    void addGuiCallback(std::shared_ptr<std::function<void()>> cb);
    void removeGuiCallback(std::shared_ptr<std::function<void()>> cb);

    bool persistant = false;
    double decayTime = 0;
private:
    void manipulated() override;

    ClientNetworkInterpreter& net;

    std::list<std::shared_ptr<std::function<void()>>> guiCallbacks {};
};
