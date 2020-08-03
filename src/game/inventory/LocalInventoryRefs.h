//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include "InventoryRefs.h"

#include "LocalInventory.h"
#include "../../net/PacketChannel.h"

class Packet;
class PacketView;
class LocalInventoryList;
class ClientNetworkInterpreter;

class LocalInventoryRefs : public InventoryRefs {
public:
    LocalInventoryRefs(SubgamePtr game, ClientNetworkInterpreter& net);
    void packetReceived(std::unique_ptr<PacketView> p);
    void init();

    void update(double delta, ClientNetworkInterpreter& net);

    virtual InventoryPtr createInventory(const std::string &inv) override;

    void watch(const std::string& inv, const std::string& list, bool persistant = false);
    void unWatch(const std::string& inv, const std::string& list);

    std::shared_ptr<LocalInventoryList> getHandList();
    void setHandList(const std::string& list);
    std::shared_ptr<LocalInventoryList> getWieldList();
    void setWieldList(const std::string& list);
    std::shared_ptr<LocalInventoryList> getCursorList();

private:
    std::shared_ptr<LocalInventoryList> handList = nullptr;
    std::shared_ptr<LocalInventoryList> wieldList = nullptr;

    ClientNetworkInterpreter& net;
    double time = 0;
};
