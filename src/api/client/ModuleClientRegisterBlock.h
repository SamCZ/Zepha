//
// Created by aurailus on 18/12/18.
//

#ifndef ZEUS_LREGISTERBLOCK_H
#define ZEUS_LREGISTERBLOCK_H

#include "../../def/LocalDefs.h"
#include "../../def/block/SelectionBox.h"
#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/block/client/LocalBlockAtlas.h"
#include <sol.hpp>

class ModuleClientRegisterBlock {
public:
    ModuleClientRegisterBlock(sol::state& lua, sol::table& zeus, LocalDefs& defs) {
        zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
            try {

            }
            catch (const char* e) {
                std::cout << Log::err << "Exception on register_block: " << e << Log::endl;
            }
        });
    }
};


#endif //ZEUS_LREGISTERBLOCK_H
