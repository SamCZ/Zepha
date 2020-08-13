//
// Created by aurailus on 2020-07-29.
//

#pragma once

#include <map>
#include <string>
#include <memory>
#include "util/CovariantPtr.h"

class Subgame;
class InventoryList;

class Inventory {
public:
    Inventory(const Inventory& o) = delete;
    explicit Inventory(SubgamePtr game, const std::string& name);

    virtual bool hasList(const std::string& name);
    virtual InventoryListPtr getList(const std::string& name);

    virtual void createList(const std::string& name, unsigned short length, unsigned short width) = 0;
    virtual void removeList(const std::string& name);


protected:
    SubgamePtr game;

    std::string name;
    std::map<std::string, std::shared_ptr<InventoryList>> lists;
};
