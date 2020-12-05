#pragma once

#include "Entity.h"

/**
 * A subclass of entity that specifically represents a Lua-controlled object.
 */
 
class LuaEntity : public virtual Entity {
public:
	LuaEntity(SubgamePtr game, DimensionPtr dim) : Entity(game, dim) {};
	
	/**
	 * Sets the display type of the entity, which determines how it should render.
	 * The parameters supplied should match one of the following patterns:
	 *
	 * - gameobject: Displays as a block or item.
	 *   argA: 'block' or 'craftitem', depending on the thing to render.
	 *   argB: The identifier of the block / item to display.
	 * - model: Displays a loaded model.
	 *   arg: The identifier of the model to display.
	 *   arg2: The texture to apply to the model.
	 *
	 * @param mode - The display type of the entity, 'gameobject' or 'model'.
	 * @param argA - A context specific argument whose purpose is determined by the display type.
	 * @param argB - A context specific argument whose purpose is determined by the display type.
	 */
	
	virtual void setAppearance(const std::string& mode, const std::string& argA, const std::string& argB) = 0;
};
