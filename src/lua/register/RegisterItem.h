//
// Created by aurailus on 2020-01-10.
//

#pragma once

#include "lua/Lua.h"
#include "game/LocalSubgame.h"
#include "game/def/BiomeDef.h"
#include "game/ServerSubgame.h"
#include "game/def/CraftItemDef.h"
#include "game/atlas/LocalDefinitionAtlas.h"
#include "game/atlas/ServerDefinitionAtlas.h"

namespace RegisterItem {
	namespace {
		
		/**
		 * Registers an item from the items table to the Definition Atlas.
		 * Generic method that works on both the client and the server, depending on if `atlas` is non-null.
		 *
		 * @param items - The items table to index.
		 * @param identifier - The identifier of the item to register.
		 * @param defs - The Definition Atlas to add the item to.
		 * @param atlas - The Texture Atlas, pass in if running on the client, otherwise pass nullptr.
		 */
		
		static void
		registerItem(sol::table items, const std::string& identifier, DefinitionAtlas& defs, TextureAtlas* atlas) {
			
			sol::table itemTable = items[identifier];
			
			// Basic item properties
			auto nameOpt = itemTable.get<sol::optional<std::string>>("name");
			auto texturesOpt = itemTable.get<sol::optional<sol::table>>("textures");
			auto maxStack = itemTable.get_or("stack", 64);
			
			if (!nameOpt) throw std::runtime_error(identifier + " is missing name property!");
			if (!texturesOpt) throw std::runtime_error(identifier + " is missing textures property!");
			
			//Convert Textures Table to Vector
			std::vector<std::string> textures;
			std::vector<std::shared_ptr<AtlasRef>> textureRefs;
			for (auto pair : *texturesOpt) {
				if (!pair.second.is<std::string>()) throw std::runtime_error("textures table has non-string value");
				textures.push_back(pair.second.as<std::string>());
				if (atlas) textureRefs.push_back((*atlas)[pair.second.as<std::string>()]);
			}
			if (textures.size() == 0) {
				textures.push_back("_missing");
				if (atlas) textureRefs.push_back((*atlas)["_missing"]);
			}
			
			// Creat the definition
			CraftItemDef* itemDef = new CraftItemDef(
				identifier,
				defs.size(),
				*nameOpt,
				maxStack,
				textures,
				textureRefs
			);
			
			if (atlas) itemDef->createModel(*atlas);
			defs.registerDef(itemDef);
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
	
	static void server(sol::table& core, ServerSubgame& game, const std::string& identifier) {
		registerItem(core["registered_items"], identifier, game.getDefs(), nullptr);
	}
	
	
	/**
	 * Client method to register an item. Calls registerItem with the necessary parameters.
	 * Registers an item to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_items', i.e. `_G['zepha']`.
	 * @param game - The server subgame instance.
	 * @param identifier - The identifier of the item to register.
	 */
	
	static void client(sol::table& core, LocalSubgame& game, const std::string& identifier) {
		registerItem(core["registered_items"], identifier, game.getDefs(), &game.textures);
	}
}