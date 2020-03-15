//
// Created by aurailus on 03/08/19.
//

#pragma once

#include <vector>

#include "LuaModFile.h"
#include "LuaModConfig.h"

class PacketView;

class LuaMod {
public:
    std::vector<LuaModFile> files {};
    LuaModConfig config {};
    std::string modPath;
    std::string serialized;

    static LuaMod fromPacket(PacketView& p);
};
