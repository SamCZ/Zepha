#pragma once

#include "lua/Lua.h"
#include "util/Util.h"
#include "lua/NoiseFromLua.h"
#include "game/LocalSubgame.h"
#include "game/ServerSubgame.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "game/atlas/ServerBiomeAtlas.h"

namespace RegisterBiome {
	namespace {
		
		/**
		 * Registers a biome from the biomes table to the Biome Atlas.
		 * Generic method that works on both the client and the server.
		 *
		 * @param biomes - The biomes table to index.
		 * @param identifier - The identifier of the biome to register.
		 * @param defs - The definition atlas to pull block definitions from.
		 * @param atlas - The Biome Atlas to add the biomes to.
		 */
		
		static void
		registerBiome(sol::table biomes, const std::string& identifier, DefinitionAtlas& defs, BiomeAtlas& atlas) {
			sol::table biomeTable = biomes[identifier];
			
			// Tags
			std::unordered_map<std::string, unsigned short> tags{};
			auto tagsTbl = biomeTable.get<sol::optional<sol::table>>("tags");
			if (tagsTbl)
				for (auto& tag : *tagsTbl)
					tags.insert({ tag.first.as<std::string>(), tag.second.as<int>() });
			
			// Environment Properties for Voronoi Diagram
			auto environment = biomeTable.get<sol::optional<sol::table>>("environment");
			if (!environment) throw std::runtime_error("biome definitions require an environment table");
			
			auto temperature = environment->get_or<float>("temperature", -2);
			auto humidity = environment->get_or<float>("humidity", -2);
			auto roughness = environment->get_or<float>("roughness", -2);
			
			if (temperature < -1 || temperature > 1)
				throw std::runtime_error("environment.temperature property is out of range or missing");
			if (humidity < 0 || humidity > 1)
				throw std::runtime_error("environment.humidity property is out of range or missing");
			if (roughness < 0 || roughness > 1)
				throw std::runtime_error("environment.roughness property is out of range or missing");
			
			// Blocks to form the environment out of
			auto blocksList = biomeTable.get<sol::optional<sol::table>>("blocks");
			if (!blocksList) throw identifier + "biome definitions require a blocks table";
			
			auto bTop = (*blocksList).get<sol::optional<std::string>>("top");
			auto bSoil = (*blocksList).get<sol::optional<std::string>>("soil");
			auto bRock = (*blocksList).get<sol::optional<std::string>>("rock");
			
			if (!bTop) throw identifier + "blocks.top property is missing";
			if (!bSoil) throw identifier + "blocks.soil property is missing";
			if (!bRock) throw identifier + "blocks.rock property is missing";
			
			// Get biome tint
			auto biomeTint = biomeTable.get<sol::optional<std::string >>("biome_tint");
			if (!biomeTint) throw identifier + "biome definitions require a biome_tint";
			
			// Get noise parameters
			let constGenerator = FastNoise::New<FastNoise::Constant>();
			constGenerator->SetValue(0);
			FastNoise::SmartNode<> heightmap = constGenerator, volume = constGenerator;
			
			auto noiseList = biomeTable.get<sol::optional<sol::table>>("noise");
			
			if (noiseList) {
				let heightObj = noiseList->get<sol::object>("heightmap");
				if (heightObj.is<sol::table>()) heightmap =
					NoiseFromLua::parse(heightObj.as<sol::table>());
				else if (heightObj.is<string>()) heightmap =
					FastNoise::NewFromEncodedNodeTree(heightObj.as<string>().data());
				
				let volumeObj = noiseList->get<sol::object>("volume");
				if (volumeObj.is<sol::table>()) volume =
					NoiseFromLua::parse(volumeObj.as<sol::table>());
				else if (volumeObj.is<string>()) volume =
					FastNoise::NewFromEncodedNodeTree(volumeObj.as<string>().data());
			}
			
			std::vector<std::shared_ptr<Structure>> schematics {};
			if (biomeTable.get<sol::optional<sol::table>>("structures"))
				for (auto s : biomeTable.get<sol::table>("structures"))
					schematics.push_back(s.second.as<std::shared_ptr<Structure>>());
			
			// Create biome definition
			BiomeDef* biomeDef = new BiomeDef();
			
			biomeDef->identifier = identifier;
			biomeDef->index = atlas.size();
			biomeDef->tags = tags;
			
			biomeDef->temperature = temperature;
			biomeDef->humidity = humidity;
			biomeDef->roughness = roughness;
			
			biomeDef->topBlock = defs.blockFromStr(*bTop).index;
			biomeDef->soilBlock = defs.blockFromStr(*bSoil).index;
			biomeDef->rockBlock = defs.blockFromStr(*bRock).index;
			
			biomeDef->heightmap = heightmap;
			biomeDef->volume = volume;
			
			biomeDef->schematics = schematics;
			
			biomeDef->tint = Util::hexToColorVec((*biomeTint));
			
			// Add biome to biomes
			atlas.registerBiome(biomeDef);
		}
	}
	
	
	/**
	 * Server method to register a biome. Calls registerBiome with the necessary parameters.
	 * Client and Server methods are the same, but are separated for consistency.
	 * Registers a biome to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_biomes', i.e. `_G['zepha']`.
	 * @param game - The server subgame instance.
	 * @param identifier - The identifier of the biome to register.
	 */
	
	static void server(sol::table& core, ServerSubgame& game, const std::string& identifier) {
		registerBiome(core.get<sol::table>("registered_biomes"), identifier, game.getDefs(), game.getBiomes());
	}
	
	
	/**
	 * Client method to register a biome. Calls registerBiome with the necessary parameters.
	 * Client and Server methods are the same, but are separated for consistency.
	 * Registers a biome to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_biomes', i.e. `_G['zepha']`.
	 * @param game - The client subgame instance.
	 * @param identifier - The identifier of the biome to register.
	 */
	
	static void client(sol::table& core, LocalSubgame& game, const std::string& identifier) {
		registerBiome(core.get<sol::table>("registered_biomes"), identifier, game.getDefs(), game.getBiomes());
	}
}
