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
#include "game/atlas/LocalDefinitionAtlas.h"
#include "game/atlas/ServerDefinitionAtlas.h"

namespace RegisterBlock {
	
	namespace {
		struct TintParserData {
			string tex;
			optional<u32> tint;
			optional<string> mask;
		};
		
		using TintParser = StringParser<TintParserData>;
		TintParser parser {};
		bool parserReady = false;
		
		/**
		 * Takes a lua selection box table list, and returns a vector of selection boxes.
		 *
		 * @param boxesTable - The lua box table list.
		 * @returns a vector of selection boxes.
		 */
		
		static std::vector<SelectionBox> parseBoxes(sol::table boxesTable) {
			std::vector<SelectionBox> boxes{};
			
			for (auto pair : boxesTable) {
				if (!pair.second.is<sol::table>()) throw std::runtime_error("must be a table");
				sol::table table = pair.second;
				
				if (table.size() != 6) throw std::runtime_error("must contain exactly 6 elements");
				boxes.emplace_back(glm::vec3{ table[1], table[2], table[3] },
					glm::vec3{ table[4], table[5], table[6] });
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
			auto modelStr = blockTable.get_or<std::string>("model", "zepha:base:block");
			auto modelOpt = blockModels.get<sol::optional<sol::table>>(modelStr);
			if (!modelOpt) throw std::runtime_error("Non-existent model \"" + modelStr + "\" specified");
			
			sol::table modelTable = *modelOpt;
			BlockModel model;
			
			// Apply basic properties
			model.culls = blockTable.get_or("culls", true);
			model.visible = blockTable.get_or("visible", true);
			
			// Convert textures and low-def textures into vectors
			auto texturesOpt = blockTable.get<sol::optional<sol::table >>("textures");
			auto ldTexturesOpt = blockTable.get<sol::optional<sol::table >>("lowdef_textures");
			
			if (!texturesOpt) throw std::runtime_error("Missing textures property");
			
			std::vector<std::string> textures;
			for (auto pair : *texturesOpt) {
				if (!pair.second.is<std::string>())
					throw std::runtime_error("textures table contains non-string value");
				textures.push_back(pair.second.as<std::string>());
			}
			if (textures.size() == 0) textures.push_back("_missing");
			
			std::vector<std::string> lowdef_textures;
			if (!ldTexturesOpt) lowdef_textures = textures;
			else {
				for (auto pair : *ldTexturesOpt) {
					if (!pair.second.is<std::string>())
						throw std::runtime_error("lowdef_textures table has non-string value!");
					lowdef_textures.push_back(pair.second.as<std::string>());
				}
			}
			if (lowdef_textures.size() == 0) lowdef_textures.push_back("_missing");
			
			// Parse through mesh mods and add them
			sol::optional<sol::table> meshModTable = modelTable.get<sol::optional<sol::table>>("mesh_mods");
			if (meshModTable) {
				for (auto& modEntry : *meshModTable) {
					auto modTable = modEntry.second.as<sol::table>();
					std::string meshMod = modTable.get_or<std::string>("type", "none");
					
					if (meshMod == "none") continue;
					else if (meshMod == "offset_x")
						model.meshMods.emplace_back(MeshMod::OFFSET_X, modTable.get_or<float>("amplitude", 1));
					else if (meshMod == "offset_y")
						model.meshMods.emplace_back(MeshMod::OFFSET_Y, modTable.get_or<float>("amplitude", 1));
					else if (meshMod == "offset_z")
						model.meshMods.emplace_back(MeshMod::OFFSET_Z, modTable.get_or<float>("amplitude", 1));
					else if (meshMod == "rotate_x")
						model.meshMods.emplace_back(MeshMod::ROTATE_X, modTable.get_or<float>("amplitude", 1));
					else if (meshMod == "rotate_y")
						model.meshMods.emplace_back(MeshMod::ROTATE_Y, modTable.get_or<float>("amplitude", 1));
					else if (meshMod == "rotate_z")
						model.meshMods.emplace_back(MeshMod::ROTATE_Z, modTable.get_or<float>("amplitude", 1));
				}
			}
			
			// Parse through all the parts and add them to the model
			auto partsOpt = modelTable.get<sol::optional<sol::table>>("parts");
			if (!partsOpt) throw std::runtime_error("blockmodel is missing parts table");
			partsOpt->for_each([&](sol::object key, sol::object value) {
				
				// Validate that variables are what we expect them to be
				if (!value.is<sol::table>()) throw std::runtime_error("meshpart must be a table");
				sol::table meshPartTable = value.as<sol::table>();
				
				auto points_optional = meshPartTable.get<sol::optional<sol::table>>("points");
				if (!points_optional) throw std::runtime_error("Meshpart is missing a points table");
				sol::table points = *points_optional;
				
				if (points.size() % 20 != 0)
					throw std::runtime_error("Points table must contain a multiple of 20 values");
				
				// Populate the Vertices and Indices vectors from the points table
				std::vector<BlockModelVertex> vertices;
				std::vector<unsigned int> indices;
				
				for (int i = 1; i <= points.size() / 5; i++) {
					int offset = (i - 1) * 5 + 1;
					
					glm::vec3 pos(points[offset], points[offset + 1], points[offset + 2]);
					glm::vec2 tex(points[offset + 3], points[offset + 4]);
					
					vertices.push_back(BlockModelVertex{ pos, {}, tex, tex, {}, {}});
				}
				
				int ind = 0;
				for (int i = 1; i <= points.size() / 20; i++) {
					indices.push_back(ind);
					indices.push_back(ind + 1);
					indices.push_back(ind + 2);
					indices.push_back(ind + 2);
					indices.push_back(ind + 3);
					indices.push_back(ind);
					ind += 4;
				}
				
				// Get the part's texture
				int tex = std::max(static_cast<int>(meshPartTable.get_or<float>("tex", 1)), 1);
				let data = parser.parse(textures[std::min(tex - 1, (int) textures.size() - 1)]);
				u32 blendInd = data.tint ? *data.tint + 1 : 0;
				
				// Add texture refs to blockModel if the textures table is provided
				std::shared_ptr<AtlasRef> textureRef = nullptr, blendMaskRef = nullptr;
				if (atlas) {
					textureRef = (*atlas)[data.tex];
					model.textureRefs.insert(textureRef);
					
					if (blendInd && data.mask) {
						blendMaskRef = (*atlas)[*data.mask];
						model.textureRefs.insert(blendMaskRef);
					}
				}
				
				// Create the meshpart object
				MeshPart meshPart(std::move(vertices), std::move(indices), textureRef, blendInd, blendMaskRef);
				
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
			BlockModel farModel;
			auto ldRender = blockTable.get_or("lowdef_render", true);
			
			if (atlas) {
				std::vector<std::shared_ptr<AtlasRef>> textureRefs;
				std::vector<unsigned int> blendInds;
				std::vector<std::shared_ptr<AtlasRef>> blendMaskRefs;
				
				for (auto i = 0; i < lowdef_textures.size(); i++) {
					let data = parser.parse(lowdef_textures[i]);
					u32 blendInd = data.tint ? *data.tint + 1 : 0;
					
					textureRefs.push_back((*atlas)[data.tex]);
					blendInds.push_back(blendInd);
					blendMaskRefs.push_back(data.mask ? (*atlas)[*data.mask] : nullptr);
				}
				
				farModel = BlockModel::createCube(textureRefs, blendInds, blendMaskRefs);
			}
			else {
				farModel = BlockModel::createCube({}, {}, {});
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
		
		static void
		addCallback(BlockDef& blockDef, sol::table& blockTable, const std::string& name, BlockDef::Callback cbType) {
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
	 * Initializes the parser with the necessary parse functions.
	 * In the future, this could be cleaned up, but as it is it's totally fine.
	 */
	
	static void initParser() {
		if (parserReady) return;
		
		parser.setUnknownFnsAreLiteral(true);
		
		parser.addFn<u32, TintParserData, optional<TintParserData>>("tint",
			[](u32 tint, TintParserData tex, optional<TintParserData> mask) {
			return TintParserData { tex.tex, tint, mask ? optional(mask->tex) : std::nullopt };
		});
		
		parser.addLiteralFn([](string tex) {
			return TintParserData { tex, std::nullopt, std::nullopt };
		});
		
		parserReady = true;
	}
	
	/**
	 * Server method to register a block. Calls registerBlock with the necessary parameters.
	 * Registers a block to the DefinitionAtlas.
	 *
	 * @param core - The core table to index for 'registered_blocks', i.e. `_G['zepha']`.
	 * @param game - The server subgame instance.
	 * @param identifier - The identifier of the block to register.
	 */
	
	static void server(sol::table& core, ServerSubgame& game, const std::string& identifier) {
		initParser();
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
	
	static void client(sol::table& core, LocalSubgame& game, const std::string& identifier) {
		initParser();
		registerBlock(core["registered_blocks"], core["registered_blockmodels"],
			identifier, game.getDefs(), &game.textures);
	}
}
