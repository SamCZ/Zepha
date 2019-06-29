//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LOCALLUAPARSER_H
#define ZEUS_LOCALLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files.h>

class LocalDefs;
class LocalWorld;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(std::string mod_root) : LuaParser(std::move(mod_root)) {};

    void init(LocalDefs& defs, LocalWorld& world);

    void loadModules(LocalDefs& defs, LocalWorld& world);
    void loadMods();
    void registerBlocks(LocalDefs& defs);

    ~LocalLuaParser() = default;
};


#endif //ZEUS_LOCALLUAPARSER_H