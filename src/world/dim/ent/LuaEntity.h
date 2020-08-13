//
// Created by aurailus on 2020-08-05.
//

#pragma once

#include "Entity.h"

class LuaEntity : public virtual Entity {
public:
    LuaEntity(SubgamePtr game, DimensionPtr dim);

    virtual void setAppearance(const std::string& dMode, const std::string& argA, const std::string& argB) = 0;
};
