#pragma once

#include <string>

#include "client/graph/Model.h"

/**
 * Base definition class, contains properties that all definition types use.
 * Inherited by BlockDef and CraftItemDef, which have specialized properties.
 */

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

