//
// Created by aurailus on 11/06/19.
//

#pragma once

#include "../LuaMod.h"
#include "../LuaParser.h"
#include "../../util/Timer.h"
#include "../../def/texture/ServerTexture.h"
#include <cute_files/cute_files.h>
#include <json/json.hpp>

using nlohmann::json;

class ServerDefs;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    void init(ServerDefs& defs, ServerWorld& world, std::string rootPath);

    void loadModules(ServerDefs& defs, ServerWorld& world);
    void loadMods(ServerDefs& defs, const std::string& rootPath);
    void registerBlocks(ServerDefs& defs);

    ~ServerLuaParser() = default;

    std::vector<LuaMod> mods;
private:
    std::list<std::string> findModDirs(const std::string& rootPath);
    std::vector<LuaMod> createLuaMods(std::list<std::string> modDirs);
    void createTextures(ServerDefs& defs);
    void handleDependencies();
    void serializeMods();

    int DoFileSandboxed(std::string file);
};

