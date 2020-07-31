//
// Created by aurailus on 2019-12-17.
//

#include "Inventory.h"

#include "../Lua.h"
#include "InventoryList.h"
#include "../../game/inventory/Inventory.h"


sol::object Api::Usertype::Inventory::add_list(sol::this_state s, std::string name, int size, int width) {
    inventory.createList(name, size, width);
    return sol::make_object<InventoryList>(s, InventoryList(inventory.getListPtr(name)));
}

sol::object Api::Usertype::Inventory::get_list(sol::this_state s, std::string name) {
    if (!inventory.hasList(name)) return sol::nil;
    return sol::make_object<InventoryList>(s, InventoryList(inventory.getListPtr(name)));
}

void Api::Usertype::Inventory::remove_list(std::string name) {
    inventory.removeList(name);
}

void Api::Usertype::Inventory::set_default_list(sol::object list) {
//    if (list.is<std::string>()) inventory.setDefaultList(list.as<std::string>());
//    else inventory.setDefaultList(list.as<sol::table>().get<std::string>("name"));
}

sol::object Api::Usertype::Inventory::get_default_list(sol::this_state s) {
    return sol::nil;
//    if (inventory.getDefaultList() == "") return sol::nil;
//    else return get_list(s, inventory.getDefaultList());
}

void Api::Usertype::Inventory::bind(State state, sol::state &lua, sol::table &core) {
    if (state == State::SERVER) lua.new_usertype<Inventory>("Inventory",
        "get_list", &Inventory::get_list,
        "add_list", &Inventory::add_list,
        "remove_list", &Inventory::remove_list,

        "set_default_list", &Inventory::set_default_list,
        "get_default_list", &Inventory::get_default_list
    );
    else lua.new_usertype<Inventory>("Inventory",
        "get_list", &Inventory::get_list
    );
}
