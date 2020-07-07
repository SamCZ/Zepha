//
// Created by aurailus on 2020-02-26.
//

#pragma once

#include "Inventory.h"

class ServerInventoryList;
class ServerDefinitionAtlas;

class InventoryRefs {
public:
    InventoryRefs(ServerDefinitionAtlas& defs, ClientList* clients);

    void update();

    std::shared_ptr<Inventory> createInv(const std::string& inv);
    std::shared_ptr<Inventory> getInv(const std::string& inv);
    std::shared_ptr<ServerInventoryList> getList(const std::string& inv, const std::string& list);

    bool addWatcher(const std::string& inv, const std::string& list, unsigned int cid);
    bool removeWatcher(const std::string& inv, const std::string& list, unsigned int cid);

    void primaryInteract(const std::string& inv, const std::string& list, unsigned short ind, unsigned int cid);
    void secondaryInteract(const std::string& inv, const std::string& list, unsigned short ind, unsigned int cid);
private:
    std::unordered_map<std::string, std::shared_ptr<Inventory>> inventories {};

    ClientList* clients;
    ServerDefinitionAtlas& defs;
    double time = 0;
};
