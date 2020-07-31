//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <map>
#include <string>
#include <memory>

#include "Inventory.h"
#include "ServerInventoryList.h"

#include "../../def/ServerSubgame.h"

class ServerClients;

class ServerInventory : public Inventory {
public:
    ServerInventory(ServerSubgame& game, const std::string& name, ServerClients& clients) :
        Inventory(game, name), clients(clients) {};

    virtual void createList(const std::string& name, unsigned short length, unsigned short width) override;
    virtual ServerInventoryList& getList(const std::string& name) override;

//    void sendDirtyLists();
private:
    ServerClients& clients;
};
