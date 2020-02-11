//
// Created by aurailus on 11/06/19.
//

#pragma once

#include "../LuaMod.h"
#include "../LuaParser.h"
#include "../../util/Timer.h"
#include "../../server/asset/ServerTexture.h"
#include <cute_files/cute_files.h>
#include <json/json.hpp>
#include <iomanip>

using nlohmann::json;

class ServerDefs;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    void init(ServerDefs& defs, ServerWorld& world, std::string rootPath);

    void loadModules(ServerDefs& defs, ServerWorld& world);
    void loadMods(ServerDefs& defs, const std::string& rootPath);
    void registerDefinitions(ServerDefs &defs);

    void update(double delta) override;

    ~ServerLuaParser() = default;

    std::vector<LuaMod> mods;
private:
    std::list<std::string> findModDirs(const std::string& rootPath);
    std::vector<LuaMod> createLuaMods(std::list<std::string> modDirs);
    void createTextures(ServerDefs& defs);
    void createModels(ServerDefs& defs);
    void handleDependencies();
    void serializeMods();

    sol::protected_function_result errorCallback(lua_State*, sol::protected_function_result errPfr);
    sol::protected_function_result DoFileSandboxed(std::string file);

    double delta = 0;
};

