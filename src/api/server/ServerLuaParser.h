//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_SERVERLUAPARSER_H
#define ZEUS_SERVERLUAPARSER_H

#include "../LuaMod.h"
#include "../LuaParser.h"
#include <cute_files/cute_files.h>
#include <json/json.hpp>

using nlohmann::json;

class ServerDefs;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    explicit ServerLuaParser(const std::string& mod_root) : LuaParser(mod_root) {};

    void init(ServerDefs& defs, ServerWorld& world);

    void loadModules(ServerDefs& defs, ServerWorld& world);
    void loadMods();
    void registerBlocks(ServerDefs& defs);

    ~ServerLuaParser() = default;
private:
    std::list<std::string> findModDirs();
    std::vector<LuaMod> createLuaMods(std::list<std::string> modDirs);
    void handleDependencies();

    int DoFileSandboxed(std::string file);

    std::vector<LuaMod> mods;
};


#endif //ZEUS_SERVERLUAPARSER_H
