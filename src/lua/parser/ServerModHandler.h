//
// Created by aurailus on 2020-02-19.
//

#pragma once

#include <list>
#include <string>
#include "../LuaMod.h"

class ServerGame;

class ServerModHandler {
public:
    void loadMods(ServerGame& defs, const std::string& path);
    void executeMods(std::function<void(std::string)> run);
    const std::vector<LuaMod>& cGetMods() const;
private:
    static std::list<std::string> findModDirectories(const std::string& path);
    static std::vector<LuaMod> initializeLuaMods(const std::list<std::string>& modDirs);
    static void loadTextures(ServerGame &defs, const std::vector<LuaMod>& mods);
    static void loadModels(ServerGame &defs, const std::vector<LuaMod>& mods);
    static void organizeDependencies(std::vector<LuaMod>& mods);
    static void serializeMods(std::vector<LuaMod>& mods);

    std::vector<LuaMod> mods {};
};
