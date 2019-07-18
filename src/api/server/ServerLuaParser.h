//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_SERVERLUAPARSER_H
#define ZEUS_SERVERLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files/cute_files.h>

class ServerDefs;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    explicit ServerLuaParser(std::string mod_root) : LuaParser(std::move(mod_root)) {};

    void init(ServerDefs& defs, ServerWorld& world);

    void loadModules(ServerDefs& defs, ServerWorld& world);
    void loadMods();
    void registerBlocks(ServerDefs& defs);

    ~ServerLuaParser() = default;
};


#endif //ZEUS_SERVERLUAPARSER_H
