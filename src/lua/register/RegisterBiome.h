#pragma once

#include <libnoise/module/add.h>
#include <libnoise/module/module.h>
#include <libnoise/module/modulebase.h>

#include "lua/Lua.h"
#include "util/Util.h"
#include "game/LocalSubgame.h"
#include "game/ServerSubgame.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "game/atlas/LocalBiomeAtlas.h"
#include "game/atlas/ServerBiomeAtlas.h"

namespace RegisterBiome {
	namespace {
		
		/**
		 * Will initialize Noise::Module instances from a lua noise module definition, and recursively initialize it's
		 * child modules as well. All modules will be added in reverse-discovered-order to the modules vector reference
		 * passed in. The top level module will always be the last element in the vector.
		 *
		 * @param modules - The vector reference to insert generated noise modules into.
		 * @param noise - The lua noise definition to parse.
		 * @returns the noise module that was parsed.
		 */
		
		static noise::module::Module* parseNoise(std::vector<noise::module::Module*>& modules, sol::table noise) {
			std::string type = noise["module"];
			
			// Modifer Modules
			if (type == "abs") {
				auto module = new noise::module::Abs();
				
				modules.push_back(module);
				return module;
			}
			else if (type == "clamp") {
				auto module = new noise::module::Clamp();
				module->SetBounds(noise.get_or<float>("low", noise::module::DEFAULT_CLAMP_LOWER_BOUND),
					noise.get_or<float>("high", noise::module::DEFAULT_CLAMP_UPPER_BOUND));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "curve") {
				auto module = new noise::module::Exponent();
				
				module->SetExponent(noise.get_or<float>("exponent", noise::module::DEFAULT_EXPONENT));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "invert") {
				auto module = new noise::module::Invert();
				
				modules.push_back(module);
				return module;
			}
			else if (type == "scale_bias") {
				auto module = new noise::module::ScaleBias();
				sol::table source = noise["source"];
				
				auto mod = parseNoise(modules, source);
				module->SetSourceModule(0, *mod);
				
				module->SetScale(noise.get_or<float>("scale", noise::module::DEFAULT_SCALE));
				module->SetBias(noise.get_or<float>("bias", noise::module::DEFAULT_BIAS));
				
				modules.push_back(module);
				return module;
			}
				// Combiner Modules
			else if (type == "add") {
				auto module = new noise::module::Add();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				
				modules.push_back(module);
				return module;
			}
			else if (type == "max") {
				auto module = new noise::module::Max();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				
				modules.push_back(module);
				return module;
			}
			else if (type == "min") {
				auto module = new noise::module::Min();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				
				modules.push_back(module);
				return module;
			}
			else if (type == "multiply") {
				auto module = new noise::module::Multiply();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				
				modules.push_back(module);
				return module;
			}
			else if (type == "power") {
				auto module = new noise::module::Power();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				
				modules.push_back(module);
				return module;
			}
				// Generator modules
			else if (type == "billow") {
				auto module = new noise::module::Billow();
				
				module->SetSeed(noise.get_or<float>("seed", 0));
				module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_BILLOW_OCTAVE_COUNT));
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_BILLOW_FREQUENCY));
				module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_BILLOW_LACUNARITY));
				module->SetPersistence(noise.get_or<float>("persistence", noise::module::DEFAULT_BILLOW_PERSISTENCE));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "checkerboard") {
				auto module = new noise::module::Checkerboard();
				
				modules.push_back(module);
				return module;
			}
			else if (type == "const") {
				auto module = new noise::module::Const();
				
				module->SetConstValue(noise.get_or<float>("value", noise::module::DEFAULT_CONST_VALUE));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "cylinders") {
				auto module = new noise::module::Cylinders();
				
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_CYLINDERS_FREQUENCY));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "ridged_multi") {
				auto module = new noise::module::RidgedMulti();
				
				module->SetSeed(noise.get_or<float>("seed", 0));
				module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_RIDGED_OCTAVE_COUNT));
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_RIDGED_FREQUENCY));
				module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_RIDGED_LACUNARITY));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "spheres") {
				auto module = new noise::module::Spheres();
				
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_SPHERES_FREQUENCY));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "perlin") {
				auto module = new noise::module::Perlin();
				
				module->SetSeed(noise.get_or<float>("seed", 0));
				module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_PERLIN_OCTAVE_COUNT));
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_PERLIN_FREQUENCY));
				module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_PERLIN_LACUNARITY));
				module->SetPersistence(noise.get_or<float>("persistence", noise::module::DEFAULT_PERLIN_PERSISTENCE));
				
				modules.push_back(module);
				return module;
			}
			else if (type == "voronoi") {
				auto module = new noise::module::Voronoi();
				
				module->SetSeed(noise.get_or<float>("seed", 0));
				module->SetDisplacement(noise.get_or<float>("displacement", 0));
				module->SetFrequency(noise.get_or<float>("frequency", 0));
				
				modules.push_back(module);
				return module;
			}
				// Selector Modules
			else if (type == "blend") {
				auto module = new noise::module::Blend();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				auto control = parseNoise(modules, noise["control"]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				module->SetSourceModule(1, *control);
				
				modules.push_back(module);
				return module;
			}
			else if (type == "select") {
				auto module = new noise::module::Select();
				sol::table sources = noise["sources"];
				
				auto mod0 = parseNoise(modules, sources[1]);
				auto mod1 = parseNoise(modules, sources[2]);
				auto control = parseNoise(modules, noise["control"]);
				module->SetSourceModule(0, *mod0);
				module->SetSourceModule(1, *mod1);
				module->SetSourceModule(1, *control);
				
				modules.push_back(module);
				return module;
			}
				// Transformer Modules
			else if (type == "turbulence") {
				auto module = new noise::module::Turbulence();
				sol::table source = noise["source"];
				
				auto mod0 = parseNoise(modules, source);
				module->SetSourceModule(0, *mod0);
				module->SetPower(noise.get_or<float>("power", noise::module::DEFAULT_TURBULENCE_POWER));
				module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_TURBULENCE_FREQUENCY));
				module->SetRoughness(noise.get_or<float>("roughness", noise::module::DEFAULT_TURBULENCE_ROUGHNESS));
				
				modules.push_back(module);
				return module;
			}
			throw std::runtime_error("Invalid noise module specified.");
		}
		
		
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
			auto noiseList = biomeTable.get<sol::optional<sol::table>>("noise");
			std::vector<noise::module::Module*> volumeModules, heightmapModules;
			
			if (noiseList) {
				if (noiseList->get<sol::optional<sol::table>>("heightmap"))
					parseNoise(heightmapModules, noiseList->get<sol::table>("heightmap"));
				else heightmapModules.push_back(new noise::module::Const);
				
				if (noiseList->get<sol::optional<sol::table>>("volume"))
					parseNoise(volumeModules, noiseList->get<sol::table>("volume"));
				else volumeModules.push_back(new noise::module::Const);
			}
			else {
				volumeModules.push_back(new noise::module::Const);
				heightmapModules.push_back(new noise::module::Const);
			}
			
			std::vector<std::shared_ptr<Schematic>> schematics {};
			if (biomeTable.get<sol::optional<sol::table>>("structures"))
				for (auto s : biomeTable.get<sol::table>("structures"))
					schematics.push_back(s.second.as<std::shared_ptr<Schematic>>());
			
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
			
			biomeDef->heightmap = heightmapModules;
			biomeDef->volume = volumeModules;
			
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
