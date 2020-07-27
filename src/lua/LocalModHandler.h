//
// Created by aurailus on 2020-02-19.
//

#pragma once

#include <functional>

#include "LuaMod.h"

class LocalModHandler {
public:
    void addLuaMod(const LuaMod& mod);
    void setModsOrder(const std::vector<std::string>& order);

    void executeMods(std::function<void(std::string)> run);
    const std::vector<LuaMod>& cGetMods() const;
private:
    std::vector<LuaMod> mods;
    std::vector<std::string> modsOrder;
};
