//
// Created by aurailus on 13/08/19.
//

#ifndef ZEUS_SREGISTERITEM_H
#define ZEUS_SREGISTERITEM_H

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_item(sol::state& lua, sol::table& zeus) {
        zeus["registered_items"] = lua.create_table();

        zeus.set_function("register_item", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            zeus["registered_items"][identifier] = data;
        });
    }
}

#endif //ZEUS_SREGISTERITEM_H
