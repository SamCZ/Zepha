//
// Created by aurailus on 17/12/18.
//

#pragma once

#include <cute_files/cute_files.h>
#include "../LuaParser.h"
#include "../LuaMod.h"

class LocalDefs;
class LocalWorld;
class GameGui;

class LocalLuaParser : public LuaParser {
public:
    void init(LocalDefs& defs, LocalWorld& world, GameGui& gui);

    void loadModules(LocalDefs& defs, LocalWorld& world, GameGui& gui);
    void loadMods();
    void registerBlocks(LocalDefs& defs);

    ~LocalLuaParser() = default;

    std::vector<LuaMod> mods;
    std::vector<std::string> modsOrder;
private:
    int DoFileSandboxed(std::string file);
};
