//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_MODULESERVERREGISTERBLOCKMODEL_H
#define ZEUS_MODULESERVERREGISTERBLOCKMODEL_H

#include "../../def/ServerDefs.h"
#include <sol.hpp>

class ModuleServerRegisterFunctions {
public:
    ModuleServerRegisterFunctions(sol::state& lua, sol::table& zeus, ServerDefs& defs) {
        zeus["registered_blockmodels"] = lua.create_table();

        zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            zeus["registered_blockmodels"][identifier] = data;
        });

        zeus["registered_blocks"] = lua.create_table();

        zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            zeus["registered_blocks"][identifier] = data;
        });
    }
};

#endif //ZEUS_MODULESERVERREGISTERBLOCKMODEL_H
