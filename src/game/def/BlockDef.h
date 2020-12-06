#pragma once

#include <string>

#include "ItemDef.h"

#include "lua/Lua.h"
#include "util/Util.h"
#include "game/def/mesh/BlockModel.h"
#include "game/def/mesh/SelectionBox.h"

/**
 * Represents a block definition, defined by Lua.
 * Contains all the necessary information about a block.
 */
 
class BlockDef : public ItemDef {
public:
	enum class Callback {
		CONSTRUCT, AFTER_CONSTRUCT,
		DESTRUCT, AFTER_DESTRUCT,
		
		PLACE, PLACE_CLIENT,
		AFTER_PLACE, AFTER_PLACE_CLIENT,
		
		BREAK, BREAK_CLIENT,
		AFTER_BREAK, AFTER_BREAK_CLIENT,
		
		INTERACT, INTERACT_CLIENT,
		HIT, HIT_CLIENT
	};
	
	BlockDef() : ItemDef{ "", "", 0, 0, ItemDef::Type::BLOCK } {};
	
	void createModel();
	
	bool hasInteraction();
	
	BlockModel model;
	BlockModel farModel;
	
	bool culls = true;
	bool solid = true;
	bool lightPropagates = false;
	
	int health = 0;
	int defense = 0;
	
	glm::ivec3 lightSource{};
	
	std::vector<SelectionBox> sBoxes{};
	std::vector<SelectionBox> cBoxes{};
	
	std::unordered_map<Callback, sol::protected_function, Util::EnumClassHash> callbacks {};
};
