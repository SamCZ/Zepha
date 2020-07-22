//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

class PacketView;
class LocalInventory;
class LocalInventoryList;
class LocalDefinitionAtlas;
class ClientNetworkInterpreter;

class LocalInventoryRefs {
public:
    typedef std::function<void(const std::string& inv, const std::string& list, unsigned short)> inv_callback_fn;

    LocalInventoryRefs(LocalDefinitionAtlas& defs, ClientNetworkInterpreter& net);
    void init();

    void update(double delta, ClientNetworkInterpreter& net);
    void packetReceived(std::unique_ptr<PacketView> p);
    void watch(const std::string& inv, const std::string& list, bool persistant = false);
    void unWatch(const std::string& inv, const std::string& list);

    std::shared_ptr<LocalInventory> getInv(const std::string& inv);
    std::shared_ptr<LocalInventoryList> getList(const std::string& inv, const std::string& list);

    std::shared_ptr<LocalInventoryList> getHandList();
    void setHandList(const std::string& list);
    std::shared_ptr<LocalInventoryList> getWieldList();
    void setWieldList(const std::string& list);
    std::shared_ptr<LocalInventoryList> getCursorList();
private:
    std::unordered_map<std::string, std::shared_ptr<LocalInventory>> inventories {};

    inv_callback_fn primaryCallback = nullptr;
    inv_callback_fn secondaryCallback = nullptr;

    std::function<void(std::string, std::string)> watchFn = nullptr;
    std::function<void(std::string, std::string)> unWatchFn = nullptr;

    std::string handList = "";
    std::string wieldList = "";

    LocalDefinitionAtlas& defs;

    double time = 0;
};
