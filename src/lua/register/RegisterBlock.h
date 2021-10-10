#pragma once

#include "lua/Lua.h"
#include "game/def/ItemDef.h"
#include "game/LocalSubgame.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "game/ServerSubgame.h"
#include "game/def/CraftItemDef.h"
#include "game/def/mesh/BlockModel.h"
#include "game/def/mesh/SelectionBox.h"
#include "game/atlas/asset/AtlasTexture.h"
#include "game/atlas/LocalDefinitionAtlas.h"
#include "game/atlas/ServerDefinitionAtlas.h"

namespace RegisterBlock {
	namespace {
		
		/**
		 * Takes a lua selection box table list, and returns a vector of selection boxes.
		 *
		 * @param boxesTable - The lua box table list.
		 * @returns a vector of selection boxes.
		 */
		
		static vec<SelectionBox> parseBoxes(sol::table boxesTable) {
			vec<SelectionBox> boxes{};
			
			for (auto pair : boxesTable) {
				if (!pair.second.is<sol::table>()) throw std::runtime_error("must be a table");
				sol::table table = pair.second;
				
				if (table.size() != 6) throw std::runtime_error("must contain exactly 6 elements");
				boxes.emplace_back(vec3 { table[1], table[2], table[3] }, vec3 { table[4], table[5], table[6] });
			}
			
			return boxes;
		}
		
		/**
		 * Creates near and far models for a block based on the passed in parameters.
		 *
		 * @param blockTable - The lua block table to find the model parameters from.
		 * @param blockModels - The lua block models table with the available block model definitions.
		 * @param atlas - The texture atlas, can be nullptr on the server to disable linking textures.
		 *
		 * @returns two block models in a pair, the first being the near model and the second being the far model.
		 */
		
		static std::pair<BlockModel, BlockModel>
		createBlockModel(sol::table blockTable, sol::table blockModels, TextureAtlas* atlas) {
			// Get the specified block model
			let modelStr = blockTable.get_or<string>("model", "zepha:base:block");
			let modelOpt = blockModels.get<optional<sol::table>>(modelStr);
			if (!modelOpt) throw std::runtime_error("Non-existent model \"" + modelStr + "\" specified");
			
			sol::table modelTable = *modelOpt;
			BlockModel model;
			
			// Apply basic properties
			model.culls = blockTable.get_or("culls", true);
			model.visible = blockTable.get_or("visible", true);
			
			// Convert textures and low-def textures into vectors
			let texturesOpt = blockTable.get<sol::optional<sol::table>>("textures");
			let ldTexturesOpt = blockTable.get<sol::optional<sol::table>>("lowdef_textures");
			
			if (!texturesOpt) throw std::runtime_error("Missing textures property");
			
			vec<string> textures;
			for (let& pair : *texturesOpt) {
				if (!pair.second.is<string>()) throw std::runtime_error(
					"textures table contains non-string value");
				textures.push_back(pair.second.as<string>());
			}
			if (textures.empty()) textures.push_back("_missing");
			
			vec<string> lowdef_textures;
			if (!ldTexturesOpt) lowdef_textures = textures;
			else {
				for (let& pair : *ldTexturesOpt) {
					if (!pair.second.is<string>()) throw std::runtime_error(
						"lowdef_textures table has non-string value!");
					lowdef_textures.push_back(pair.second.as<string>());
				}
			}
			if (lowdef_textures.empty()) lowdef_textures.push_back("_missing");
			
			// Parse through mesh mods and add them
			optional<sol::table> meshModTable = modelTable.get<optional<sol::table>>("mesh_mods");
			if (meshModTable) {
				for (auto& modEntry : *meshModTable) {
					auto modTable = modEntry.second.as<sol::table>();
					string meshMod = modTable.get_or<string>("type", "none");
					
					if (meshMod == "none") continue;
					else if (meshMod == "offset_x")
						model.meshMods.emplace_back(MeshMod::OFFSET_X, modTable.get_or<f32>("amplitude", 1));
					else if (meshMod == "offset_y")
						model.meshMods.emplace_back(MeshMod::OFFSET_Y, modTable.get_or<f32>("amplitude", 1));
					else if (meshMod == "offset_z")
						model.meshMods.emplace_back(MeshMod::OFFSET_Z, modTable.get_or<f32>("amplitude", 1));
					else if (meshMod == "rotate_x")
						model.meshMods.emplace_back(MeshMod::ROTATE_X, modTable.get_or<f32>("amplitude", 1));
					else if (meshMod == "rotate_y")
						model.meshMods.emplace_back(MeshMod::ROTATE_Y, modTable.get_or<f32>("amplitude", 1));
					else if (meshMod == "rotate_z")
						model.meshMods.emplace_back(MeshMod::ROTATE_Z, modTable.get_or<f32>("amplitude", 1));
				}
			}
			
			// Parse through all the parts and add them to the model
			let partsOpt = modelTable.get<optional<sol::table>>("parts");
			if (!partsOpt) throw std::runtime_error("blockmodel is missing parts table");
			partsOpt->for_each([&](sol::object key, sol::object value) {
				
				// Validate that variables are what we expect them to be
				if (!value.is<sol::table>()) throw std::runtime_error("meshpart must be a table");
				sol::table meshPartTable = value.as<sol::table>();
				
				let points_optional = meshPartTable.get<sol::optional<sol::table>>("points");
				if (!points_optional) throw std::runtime_error("Meshpart is missing a points table");
				sol::table points = *points_optional;
				
				if (points.size() % 20 != 0)
					throw std::runtime_error("Points table must contain a multiple of 20 values");
				
				// Populate the Vertices and Indices vectors from the points table
				vec<BlockModelVertex> vertices;
				vec<u32> indices;
				
				for (u32 i = 1; i <= points.size() / 5; i++) {
					u32 offset = (i - 1) * 5 + 1;
					
					vec3 pos(points[offset], points[offset + 1], points[offset + 2]);
					vec2 tex(points[offset + 3], points[offset + 4]);
					
					vertices.push_back(BlockModelVertex { pos, {}, tex, tex, {}, {} });
				}
				
				u32 ind = 0;
				for (u32 i = 1; i <= points.size() / 20; i++) {
					indices.push_back(ind);
					indices.push_back(ind + 1);
					indices.push_back(ind + 2);
					indices.push_back(ind + 2);
					indices.push_back(ind + 3);
					indices.push_back(ind);
					ind += 4;
				}
				
				// Get texture, and add texture refs to blockModel if the atlas is provided
				u32 tex = std::max(meshPartTable.get_or<u32>("tex", 1), 1u);
				
				optional<u32> tintInd {};
				optional<AtlasRef> textureRef {};
				optional<AtlasRef> tintTextureRef = {};
				
				if (atlas) {
					textureRef = (*atlas)[textures[std::min(tex - 1, static_cast<u32>(textures.size()) - 1)]];
					model.textures.push_back(*textureRef);
					tintInd = (*textureRef)->getTintInd();
					tintTextureRef = (*textureRef)->getTintMask();
				}
				
				// Create the meshpart object
				MeshPart meshPart(std::move(vertices), std::move(indices), textureRef, tintInd, tintTextureRef);
				
				// Add the shader mod, if it exists
				sol::optional<sol::table> shaderModTable = meshPartTable.get<sol::optional<sol::table>>("shader_mod");
				if (shaderModTable) {
					std::string shaderMod = shaderModTable->get_or<std::string>("type", "none");
					
					if (shaderMod == "none") meshPart.shaderMod = ShaderMod::NONE;
					else if (shaderMod == "rotate_x") {
						meshPart.shaderMod = ShaderMod::ROTATE_X;
						meshPart.modValue = (*shaderModTable).get_or<float>("speed", 1);
					}
					else if (shaderMod == "rotate_y") {
						meshPart.shaderMod = ShaderMod::ROTATE_Y;
						meshPart.modValue = (*shaderModTable).get_or<float>("speed", 1);
					}
					else if (shaderMod == "rotate_z") {
						meshPart.shaderMod = ShaderMod::ROTATE_Z;
						meshPart.modValue = (*shaderModTable).get_or<float>("speed", 1);
					}
					else if (shaderMod == "sway_attached") {
						meshPart.shaderMod = ShaderMod::SWAY_ATTACHED;
						meshPart.modValue = (*shaderModTable).get_or<float>("amplitude", 1);
					}
					else if (shaderMod == "sway_full_block") {
						meshPart.shaderMod = ShaderMod::SWAY_FULL_BLOCK;
						meshPart.modValue = (*shaderModTable).get_or<float>("amplitude", 1);
					}
				}
				
				//Add the meshpart to the proper face of the model
				std::string face = meshPartTable.get_or<std::string>("face", "nocull");
				
				EVec d =
					face == "top" ? EVec::TOP :
					face == "bottom" ? EVec::BOTTOM :
					face == "left" ? EVec::LEFT :
					face == "right" ? EVec::RIGHT :
					face == "front" ? EVec::FRONT :
					face == "back" ? EVec::BACK :
					face == "nocull" ? EVec::NO_CULL :
					EVec::INVALID;
				
				if (d == EVec::INVALID) throw std::runtime_error("face value is unrecognized");
				model.parts[static_cast<int>(d)].push_back(meshPart);
			});
			
			// Create the far model
			BlockModel farModel {};
			auto ldRender = blockTable.get_or("lowdef_render", true);
			
			if (atlas) {
				vec<std::tuple<sptr<AtlasTexture>, optional<u32>, optional<sptr<AtlasTexture>>>> modelData;
				
				for (let i = 0; i < lowdef_textures.size(); i++) {
					let textureRef = (*atlas)[lowdef_textures[i]];
					modelData.emplace_back(textureRef, textureRef->getTintInd(), textureRef->getTintMask());
				}
				
				farModel = BlockModel { modelData };
			}
			
			farModel.culls = ldRender;
			farModel.visible = ldRender;
			
			return { model, farModel };
		}
		
		
		/**
		 * Attempts to add a callback of the type specified from the block table provided
		 * to the block definition provided. Does nothing if the block table doesn't have a callback of said type.
		 *
		 * @param blockDef - The block definition to add the callback to.
		 * @param blockTable - The lua block table to get the callback from.
		 * @param name - The name of the callback to look for in the lua table.
		 * @param cbType - The type to register the callback as.
		 */
		
		static void addCallback(BlockDef& blockDef, sol::table& blockTable,
			const string& name, BlockDef::Callback cbType) {
			auto cb = blockTable.get<sol::optional<sol::protected_function>>(name);
			if (cb) blockDef.callbacks.insert({ cbType, *cb });
		}
		
		
		/**
		 * Registers a block from the blocks table to the Definition Atlas.
		 * Generic method that works on both the client and the server, depending on if `atlas` is non-null.
		 *
		 * @param blocks - The blocks table to index.
		 * @param blockModels - The blockmodels table for indexing block models.
		 * @param identifier - The identifier of the block to register.
		 * @param defs - The Definition Atlas to add the block to.
		 * @param atlas - The Texture Atlas, pass in if running on the client, otherwise pass nullptr.
		 */
		
		static void registerBlock(sol::table blocks, sol::table blockModels,
			const std::string& identifier, DefinitionAtlas& defs, TextureAtlas* atlas) {
			
			sol::table blockTable = blocks[identifier];
			
			// Basic Block Properties
			auto nameOpt = blockTable.get<sol::optional<std::string>>("name");
			if (!nameOpt) throw std::runtime_error(identifier + " is missing name property!");
			
			bool culls = blockTable.get_or("culls", true);
			bool solid = blockTable.get_or("solid", true);
			bool lightPropagates = blockTable.get_or("light_propagates", false);
			auto maxStack = blockTable.get_or("stack", 64);
			
			unsigned int health = INT32_MAX, defense = 0;
			auto toolOpt = blockTable.get<sol::optional<sol::table>>("tool_props");
			if (toolOpt) {
				health = toolOpt->get_or<unsigned int>("health", INT32_MAX);
				defense = toolOpt->get_or<unsigned int>("defense", 0);
			}
			
			glm::vec3 lightSource{};
			if (blockTable.get<sol::optional<sol::table>>("light_source")) {
				auto light = blockTable.get<sol::table>("light_source");
				lightSource = { light[1], light[2], light[3] };
			}
			else if (blockTable.get_or<float>("light_source", -1) != -1) {
				auto light = blockTable.get<float>("light_source");
				lightSource = { light, light, light };
			}
			
			// Parse through selection boxes and collision boxes
			auto selectionOpt = blockTable.get<sol::optional<sol::table>>("selection_box");
			auto collisionOpt = blockTable.get<sol::optional<sol::table>>("collision_box");
			
			std::vector<SelectionBox> selectionBoxes{};
			try {
				if (selectionOpt) selectionBoxes = parseBoxes(*selectionOpt);
				else selectionBoxes.emplace_back(glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 });
			}
			catch (const char* error) {
				throw std::string("selection boxes " + std::string(error)).c_str();
			}
			
			std::vector<SelectionBox> collisionBoxes{};
			try { if (collisionOpt) collisionBoxes = parseBoxes(*collisionOpt); }
			catch (const char* error) { throw std::string("collision boxes " + std::string(error)).c_str(); }
			if (collisionBoxes.size() == 0) collisionBoxes.emplace_back(glm::vec3{ 0, 0, 0 }, glm::vec3{ 1, 1, 1 });
			
			// Create the block model
			std::pair<BlockModel, BlockModel> models = createBlockModel(blockTable, blockModels, atlas);
			
			BlockDef* def = new BlockDef();
			def->identifier = identifier;
			def->name = *nameOpt;
			def->index = defs.size();
			
			def->culls = culls;
			def->solid = solid;
			def->lightSource = lightSource;
			def->lightPropagates = lightPropagates;
			
			def->health = health;
			def->defense = defense;
			
			def->maxStack = maxStack;
			
			def->model = models.first;
			def->farModel = models.second;
			
			def->sBoxes = std::move(selectionBoxes);
			def->cBoxes = std::move(collisionBoxes);
			
			// Create entity model
			if (atlas) def->createModel();
			
			// Bind Callbacks
			addCallback(*def, blockTable, "on_construct", BlockDef::Callback::CONSTRUCT);
			addCallback(*def, blockTable, "after_construct", BlockDef::Callback::AFTER_CONSTRUCT);
			
			addCallback(*def, blockTable, "on_destruct", BlockDef::Callback::DESTRUCT);
			addCallback(*def, blockTable, "after_destruct", BlockDef::Callback::AFTER_DESTRUCT);
			
			addCallback(*def, blockTable, "on_place", BlockDef::Callback::PLACE);
			addCallback(*def, blockTable, "on_place_client", BlockDef::Callback::PLACE_CLIENT);
			
			addCallback(*def, blockTable, "after_place", BlockDef::Callback::AFTER_PLACE);
			addCallback(*def, blockTable, "after_place_client", BlockDef::Callback::AFTER_PLACE_CLIENT);
			
			addCallback(*def, blockTable, "on_break", BlockDef::Callback::BREAK);
			addCallback(*def, blockTable, "on_break_client", BlockDef::Callback::BREAK_CLIENT);
			
			addCallback(*def, blockTable, "after_break", BlockDef::Callback::AFTER_BREAK);
			addCallback(*def, blockTable, "after_break_client", BlockDef::Callback::AFTER_BREAK_CLIENT);
			
			addCallback(*def, blockTable, "on_interact", BlockDef::Callback::INTERACT);
			addCallback(*def, blockTable, "on_interact_client", BlockDef::Callback::INTERACT_CLIENT);
			
			// Add Block Definition to the Atlas
			defs.registerDef(def);
		}
	}
	
	/**
	 * Server method to register a block. Calls registerBlock with the necessary parameters.
	 * Registers a block to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_blocks', i.e. `_G['zepha']`.
	 * @param game - The server subgame instance.
	 * @param identifier - The identifier of the block to register.
	 */
	
	static void server(sol::table& core, ServerSubgame& game, const string& identifier) {
		registerBlock(core["registered_blocks"], core["registered_blockmodels"],
			identifier, game.getDefs(), nullptr);
	}
	
	/**
	 * Client method to register a block. Calls registerBlock with the necessary parameters.
	 * Registers a block to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_blocks', i.e. `_G['zepha']`.
	 * @param game - The client subgame instance.
	 * @param identifier - The identifier of the block to register.
	 */
	
	static void client(sol::table& core, LocalSubgame& game, const string& identifier) {
		registerBlock(core["registered_blocks"], core["registered_blockmodels"],
			identifier, game.getDefs(), &game.textures);
	}
}
