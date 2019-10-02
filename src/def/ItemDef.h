//
// Created by aurailus on 11/08/19.
//

#pragma once

#include <c++/5/string>

class ItemDef {
public:
    enum class Type {
        INVALID,
        BLOCK,
        CRAFTITEM
    };

    std::string identifier = "";
    std::string name = "";
    unsigned int index = 0;

    Type type = Type::INVALID;
};

