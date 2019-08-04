//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LOCALLUAPARSER_H
#define ZEUS_LOCALLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files/cute_files.h>

class LocalDefs;
class LocalWorld;
class GameGui;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(std::string mod_root) : LuaParser(std::move(mod_root)) {};

    void init(LocalDefs& defs, LocalWorld& world, GameGui& gui);

    void loadModules(LocalDefs& defs, LocalWorld& world, GameGui& gui);
    void loadMods();
    void registerBlocks(LocalDefs& defs);

    ~LocalLuaParser() = default;
private:
    int DoFileSandboxed(std::string file);
};


#endif //ZEUS_LOCALLUAPARSER_H