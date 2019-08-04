//
// Created by aurailus on 03/08/19.
//

#ifndef ZEUS_LUAMOD_H
#define ZEUS_LUAMOD_H

#include <vector>
#include "LuaModFile.h"
#include "LuaModConfig.h"

class LuaMod {
public:
    std::vector<LuaModFile> files {};
    LuaModConfig config {};
    std::string serialized;
};

#endif //ZEUS_LUAMOD_H
