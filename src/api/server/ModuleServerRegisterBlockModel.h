//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_MODULESERVERREGISTERBLOCKMODEL_H
#define ZEUS_MODULESERVERREGISTERBLOCKMODEL_H

#include "../../def/ServerDefs.h"
#include <sol.hpp>

class ModuleServerRegisterBlockModel {
public:
    ModuleServerRegisterBlockModel(sol::state& lua, sol::table& zeus, ServerDefs& defs) {
        //Define registered_blockmodels table
        zeus["registered_blockmodels"] = lua.create_table();

        zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
            try {
                if (identifier.length() == 0) throw "No Identifier";
                zeus["registered_blockmodels"][identifier] = data;
            }
            catch (const std::string &e) {
                std::cout << Log::err << "Exception on register_blockmodel: " << e << Log::endl;
            }
        });
    }
};

#endif //ZEUS_MODULESERVERREGISTERBLOCKMODEL_H
