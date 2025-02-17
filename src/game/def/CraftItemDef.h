#pragma once

#include "ItemDef.h"

#include "lua/Lua.h"

class AtlasTexture;
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
	
	CraftItemDef(): ItemDef(ItemDef::Type::CRAFTITEM) {};
	
	CraftItemDef(const string& identifier, const string& name, u16 maxStackSize,
		const vec<string>& textures, const vec<sptr<AtlasTexture>>& texturesRefs);
	
	CraftItemDef(const string& identifier, u16 index, const string& name,
		u16 maxStackSize, const vec<string>& textures, const vec<sptr<AtlasTexture>>& texturesRefs);
	
	void createModel(TextureAtlas& atlas);
	
	bool hasUse();
	
	vec<string> textures {};
	vec<sptr<AtlasTexture>> textureRefs {};
	
	std::unordered_map<Callback, sol::protected_function, Util::EnumClassHash> callbacks {};
};
