//
// Created by aurailus on 2020-02-24.
//

#pragma once


#include "LocalInventoryList.h"
#include "../../def/DefinitionAtlas.h"
#include "../scene/net/ClientNetworkInterpreter.h"

class LocalInventory {
public:
    typedef std::function<void(const std::string& inv, const std::string& list, unsigned short)> inv_callback_fn;

    LocalInventory(DefinitionAtlas& defs, const std::string& name,
        inv_callback_fn primaryCallback, inv_callback_fn secondaryCallback) :
        defs(defs), name(name), primaryCallback(primaryCallback), secondaryCallback(secondaryCallback) {};

    void createList(std::string name, unsigned short length, unsigned short width, bool maintain = false);
    std::shared_ptr<LocalInventoryList> operator[](std::string name);

    bool pruneLists(ClientNetworkInterpreter& net, double time);

    DefinitionAtlas& defs;
private:
    std::string name;
    std::unordered_map<std::string, std::pair<double, std::shared_ptr<LocalInventoryList>>> lists;

    inv_callback_fn primaryCallback;
    inv_callback_fn secondaryCallback;
};
