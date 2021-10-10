#pragma once

#include "lua/Lua.h"
#include "game/LocalSubgame.h"
#include "game/def/BiomeDef.h"
#include "game/ServerSubgame.h"
#include "game/def/CraftItemDef.h"
#include "game/atlas/asset/AtlasTexture.h"
#include "game/atlas/LocalDefinitionAtlas.h"
#include "game/atlas/ServerDefinitionAtlas.h"

namespace RegisterItem {
	namespace {
		
		/**
		 * Attempts to add a callback of the type specified from the block table provided
		 * to the block definition provided. Does nothing if the block table doesn't have a callback of said type.
		 *
		 * @param blockDef - The block definition to add the callback to.
		 * @param blockTable - The lua block table to get the callback from.
		 * @param name - The name of the callback to look for in the lua table.
		 * @param cbType - The type to register the callback as.
		 */
		
		static void addCallback(CraftItemDef& itemDef, sol::table& itemTable,
			const string& name, CraftItemDef::Callback cbType) {
			
			auto cb = itemTable.get<optional<sol::protected_function>>(name);
			if (cb) itemDef.callbacks.insert({ cbType, *cb });
		}
		
		
		/**
		 * Registers an item from the items table to the Definition Atlas.
		 * Generic method that works on both the client and the server, depending on if `atlas` is non-null.
		 *
		 * @param items - The items table to index.
		 * @param identifier - The identifier of the item to register.
		 * @param defs - The Definition Atlas to add the item to.
		 * @param atlas - The Texture Atlas, pass in if running on the client, otherwise pass nullptr.
		 */
		
		static void registerItem(sol::table items, const string& identifier,
			DefinitionAtlas& defs, TextureAtlas* atlas) {
			sol::table itemTable = items[identifier];
			
			// Basic item properties
			let nameOpt = itemTable.get<optional<string>>("name");
			let texturesOpt = itemTable.get<optional<sol::table>>("textures");
			let maxStack = itemTable.get_or("stack", 64);
			
			if (!nameOpt) throw std::runtime_error(identifier + " is missing name property!");
			if (!texturesOpt) throw std::runtime_error(identifier + " is missing textures property!");
			
			//Convert Textures Table to Vector
			vec<string> textures;
			vec<sptr<AtlasTexture>> textureRefs;
			for (auto pair : *texturesOpt) {
				if (!pair.second.is<string>()) throw std::runtime_error("textures table has non-string value");
				textures.push_back(pair.second.as<string>());
				if (atlas) textureRefs.push_back((*atlas)[pair.second.as<string>()]);
			}
			if (textures.size() == 0) {
				textures.push_back("_missing");
				if (atlas) textureRefs.push_back((*atlas)["_missing"]);
			}
			
			// Creat the definition
			CraftItemDef* def = new CraftItemDef(
				identifier,
				defs.size(),
				*nameOpt,
				maxStack,
				textures,
				textureRefs
			);
			
			// Bind Callbacks
			addCallback(*def, itemTable, "on_use", CraftItemDef::Callback::USE);
			addCallback(*def, itemTable, "on_use_client", CraftItemDef::Callback::USE_CLIENT);
			
			addCallback(*def, itemTable, "on_hit", CraftItemDef::Callback::HIT);
			addCallback(*def, itemTable, "on_hit_client", CraftItemDef::Callback::HIT_CLIENT);
			
			// Add Item Definition to the Atlas
			if (atlas) def->createModel(*atlas);
			defs.registerDef(def);
		}
	}
	
	/**
	 * Server method to register an item. Calls registerItem with the necessary parameters.
	 * Registers an item to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_items', i.e. `_G['zepha']`.
	 * @param game - The server subgame instance.
	 * @param identifier - The identifier of the item to register.
	 */
	
	static void server(sol::table& core, ServerSubgame& game, const string& identifier) {
		registerItem(core["registered_items"], identifier, game.getDefs(), nullptr);
	}
	
	/**
	 * Client method to register an item. Calls registerItem with the necessary parameters.
	 * Registers an item to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_items', i.e. `_G['zepha']`.
	 * @param game - The client subgame instance.
	 * @param identifier - The identifier of the item to register.
	 */
	
	static void client(sol::table& core, LocalSubgame& game, const string& identifier) {
		registerItem(core["registered_items"], identifier, game.getDefs(), &game.textures);
	}
}