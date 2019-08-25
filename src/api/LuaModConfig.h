//
// Created by aurailus on 03/08/19.
//

#pragma once

#include <string>
#include <vector>

struct LuaModConfig {
    std::string name;
    std::string description;
    std::string version;
    std::vector<std::string> depends;
};
