//
// Created by aurailus on 03/08/19.
//

#ifndef ZEUS_LUAMOD_H
#define ZEUS_LUAMOD_H

#include <vector>
#include "LuaModFile.h"
#include "LuaModConfig.h"
#include "../util/net/Packet.h"

class LuaMod {
public:
    std::vector<LuaModFile> files {};
    LuaModConfig config {};
    std::string modPath;
    std::string serialized;

    static LuaMod fromPacket(const Packet& p);
};

#endif //ZEUS_LUAMOD_H
