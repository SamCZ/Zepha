//
// Created by aurailus on 2020-07-29.
//

#pragma once

#include <map>
#include <string>
#include <memory>

class Subgame;
class InventoryList;

class Inventory {
public:
    Inventory(const Inventory& o) = delete;
    explicit Inventory(Subgame& game, const std::string& name);

    virtual bool hasList(const std::string& name);
    virtual InventoryList& getList(const std::string& name);
    std::shared_ptr<InventoryList> getListPtr(const std::string& name);

    virtual void createList(const std::string& name, unsigned short length, unsigned short width) = 0;
    virtual void removeList(const std::string& name);


protected:
    Subgame& game;

    std::string name;
    std::map<std::string, std::shared_ptr<InventoryList>> lists;
};
