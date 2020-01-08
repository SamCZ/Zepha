//
// Created by aurailus on 2020-01-07.
//

#pragma once

#include <string>
#include "../../../def/LocalDefs.h"
#include "../../../game/entity/Entity.h"

class ServerLocalLuaEntity {
public:
    ServerLocalLuaEntity(unsigned int id, LocalDefs& defs, const std::string& appearance, const std::string& arg1, const std::string& arg2);

    void setDisplayType(const std::string& type, const std::string& arg, const std::string& arg2);

    LocalDefs& defs;
    std::unique_ptr<Entity> entity = std::make_unique<Entity>();
    unsigned int id;

    std::string displayType {};
    std::string displayArg1 {};
    std::string displayArg2 {};
};
