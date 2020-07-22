//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <map>
#include <string>
#include <memory>
#include "ServerInventoryList.h"

class ClientList;

class Inventory {
public:
    Inventory(DefinitionAtlas& defs, ClientList* clients, const std::string& name) : defs(defs), clients(clients), name(name) {};

    void sendDirtyLists();

    void createList(std::string name, unsigned short length, unsigned short width);
    std::shared_ptr<ServerInventoryList> operator[](std::string name);
    void removeList(std::string name);

    void setDefaultList(const std::string& name);
    std::string getDefaultList();

    DefinitionAtlas& defs;
    std::string name;
private:
    ClientList* clients;
    std::map<std::string, std::shared_ptr<ServerInventoryList>> lists;
    std::string defaultList = "";
};
