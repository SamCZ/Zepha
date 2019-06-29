//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_MODULESERVERREGISTERBLOCKMODEL_H
#define ZEUS_MODULESERVERREGISTERBLOCKMODEL_H

#include <sol.hpp>

namespace ServerApi {
    void register_block(sol::state& lua, sol::table& zeus) {
        zeus["registered_blocks"] = lua.create_table();

        zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            zeus["registered_blocks"][identifier] = data;
        });
    }
}

#endif //ZEUS_MODULECLIENTREGISTERBLOCKMODEL_H
