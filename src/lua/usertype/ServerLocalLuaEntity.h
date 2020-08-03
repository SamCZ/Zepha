//
// Created by aurailus on 2020-01-07.
//

#pragma once

#include <string>

#include "../../util/CovariantPtr.h"
#include "../../game/entity/DrawableEntity.h"

class LocalSubgame;

class ServerLocalLuaEntity {
public:
    ServerLocalLuaEntity(unsigned int id, SubgamePtr game, const std::string& appearance, const std::string& arg1, const std::string& arg2);

    void setDisplayType(const std::string& type, const std::string& arg, const std::string& arg2);

    SubgamePtr game;
    std::unique_ptr<DrawableEntity> entity = std::make_unique<DrawableEntity>();
    unsigned int id;

    std::string displayType {};
    std::string displayArg1 {};
    std::string displayArg2 {};
};
