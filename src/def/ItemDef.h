//
// Created by aurailus on 11/08/19.
//

#pragma once

#include <string>

#include "../game/entity/Model.h"

class ItemDef {
public:
    ItemDef(const ItemDef& o) = delete;

    enum class Type {
        INVALID,
        BLOCK,
        CRAFTITEM
    };

    std::string identifier = "";
    std::string name = "";
    unsigned int index = 0;

    unsigned short maxStackSize;

    Type type = Type::INVALID;

    std::shared_ptr<Model> entityModel = std::make_shared<Model>();
};

