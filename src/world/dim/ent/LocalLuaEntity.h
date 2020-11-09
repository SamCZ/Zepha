//
// Created by aurailus on 2020-08-05.
//

#pragma once

#include "LuaEntity.h"
#include "DrawableEntity.h"

class LocalLuaEntity : public virtual DrawableEntity, public LuaEntity {
	public:
	LocalLuaEntity(SubgamePtr game, DimensionPtr dim);
	
	virtual void setAppearance(const std::string& dMode, const std::string& argA, const std::string& argB) override;
};
