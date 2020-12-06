#pragma once

#include <vector>

#include "lua/Lua.h"
#include "ItemDef.h"

class AtlasRef;
class TextureAtlas;

/**
 * Represents an item definition, defined by Lua.
 * Contains all the necessary information about a item.
 */

class CraftItemDef : public ItemDef {
public:
	enum class Callback {
		USE, USE_CLIENT,
		HIT, HIT_CLIENT
	};
	
	CraftItemDef() : ItemDef{ "", "", 0, 0, ItemDef::Type::CRAFTITEM } {};
	
	CraftItemDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
		const std::vector<std::string>& textures, const std::vector<std::shared_ptr<AtlasRef>>& textureRefs);
	
	CraftItemDef(const std::string& identifier, unsigned int index, const std::string& name,
		unsigned short maxStackSize, const std::vector<std::string>& textures,
		const std::vector<std::shared_ptr<AtlasRef>>& textureRefs);
	
	void createModel(TextureAtlas& atlas);
	
	bool hasUse();
	
	std::vector<std::string> textures {};
	std::vector<std::shared_ptr<AtlasRef>> textureRefs {};
	
	std::unordered_map<Callback, sol::protected_function, Util::EnumClassHash> callbacks {};
};
