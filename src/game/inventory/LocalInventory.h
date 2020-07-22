//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

class DefinitionAtlas;
class LocalInventoryList;
class ClientNetworkInterpreter;

class LocalInventory {
public:
    typedef std::function<void(const std::string& inv, const std::string& list, unsigned short)> inv_callback_fn;

    LocalInventory(DefinitionAtlas& defs, const std::string& name,
        inv_callback_fn primaryCallback, inv_callback_fn secondaryCallback) :
        defs(defs), name(name), primaryCallback(primaryCallback), secondaryCallback(secondaryCallback) {};

    void createList(const std::string& name, unsigned short length, unsigned short width, bool persistant = false);
    void removeList(const std::string& name);
    std::shared_ptr<LocalInventoryList> operator[](std::string name);

    void setPersistant(const std::string& list, bool persistant);
    bool pruneLists(ClientNetworkInterpreter& net, double time);

    DefinitionAtlas& defs;
private:
    std::string name;
    std::unordered_map<std::string, std::pair<double, std::shared_ptr<LocalInventoryList>>> lists;

    inv_callback_fn primaryCallback;
    inv_callback_fn secondaryCallback;
};
