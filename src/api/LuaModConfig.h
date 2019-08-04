//
// Created by aurailus on 03/08/19.
//

#ifndef ZEUS_LUAMODCONFIG_H
#define ZEUS_LUAMODCONFIG_H

#include <string>
#include <vector>

struct LuaModConfig {
    std::string name;
    std::string description;
    std::string version;
    std::vector<std::string> depends;
};

#endif //ZEUS_LUAMODCONFIG_H
