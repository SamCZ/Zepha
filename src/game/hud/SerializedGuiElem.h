//
// Created by aurailus on 2019-11-03.
//

#pragma once

#include <vector>
#include <string>
#include <map>

struct SerializedGuiElem {
    std::string type;
    std::string key;
    std::map<std::string, std::string> tokens;
    std::vector<SerializedGuiElem> children;
};