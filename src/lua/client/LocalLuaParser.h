//
// Created by aurailus on 17/12/18.
//

#pragma once

#include <cute_files/cute_files.h>
#include "../LuaParser.h"
#include "../LuaMod.h"
#include "../LuaInputManager.h"

class LocalDefs;
class LocalWorld;
class Player;

class LocalLuaParser : public LuaParser {
public:
    void init(LocalDefs& defs, LocalWorld& world, Player& player);

    void loadModules(LocalDefs& defs, LocalWorld& world, Player& player);
    void loadMods();
    void registerDefinitions(LocalDefs &defs);

    void update(double delta, bool* keys);

    ~LocalLuaParser() = default;

    std::vector<LuaMod> mods;
    std::vector<std::string> modsOrder;
private:
    sol::protected_function_result DoFileSandboxed(std::string file);

    double delta = 0;
    LuaInputManager manager;
};
