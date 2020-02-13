//
// Created by aurailus on 2020-01-10.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../def/LocalDefs.h"
#include "../../def/ServerDefs.h"
#include "../../def/gen/BiomeDef.h"

namespace RegisterBlocks {

    static std::vector<SelectionBox> parseBoxes(sol::table boxesTable) {
        std::vector<SelectionBox> boxes {};

        for (auto pair : boxesTable) {
            if (!pair.second.is<sol::table>()) throw "must be a table";
            sol::table table = pair.second;

            if (table.size() != 6) throw "must contain exactly 6 elements";
            boxes.emplace_back(glm::vec3 {table[1], table[2], table[3]}, glm::vec3 {table[4], table[5], table[6]});
        }

        return boxes;
    }

    static std::pair<BlockModel, BlockModel> createBlockModel(sol::table blockTable, sol::table blockModels, TextureAtlas* atlas) {
        // Get the specified block model
        auto modelStr = blockTable.get_or<std::string>("model", "default:cube");
        auto modelOpt = blockModels.get<sol::optional<sol::table>>(modelStr);
        if (!modelOpt) throw "Non-existent model \"" + modelStr + "\" specified";

        sol::table modelTable = *modelOpt;
        BlockModel model;

        // Apply basic properties
        model.culls = blockTable.get_or("culls", true);
        model.visible = blockTable.get_or("visible", true);

        // Convert textures and low-def textures into vectors
        auto texturesOpt   = blockTable.get<sol::optional<sol::table >>("textures");
        auto ldTexturesOpt = blockTable.get<sol::optional<sol::table >>("lowdef_textures");

        if (!texturesOpt) throw "Missing textures property";

        std::vector<std::string> textures;
        for (auto pair : *texturesOpt) {
            if (!pair.second.is<std::string>()) throw "textures table contains non-string value";
            textures.push_back(pair.second.as<std::string>());
        }
        if (textures.size() == 0) textures.push_back("_missing");

        std::vector<std::string> lowdef_textures;
        if (!ldTexturesOpt) lowdef_textures = textures;
        else {
            for (auto pair : *ldTexturesOpt) {
                if (!pair.second.is<std::string>()) throw "lowdef_textures table has non-string value!";
                lowdef_textures.push_back(pair.second.as<std::string>());
            }
        }
        if (lowdef_textures.size() == 0) lowdef_textures.push_back("_missing");

        // Parse through mesh mods and add them
        sol::optional<sol::table> meshModTable = modelTable.get<sol::optional<sol::table>> ("mesh_mods");
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
            }
        }

        // Parse through all of the parts and add them to the model
        auto partsOpt = modelTable.get<sol::optional<sol::table>>("parts");
        if (!partsOpt) throw "blockmodel is missing parts table";
        partsOpt->for_each([&](sol::object key, sol::object value) {

            // Validate that variables are what we expect them to be
            if (!value.is<sol::table>()) throw "meshpart must be a table";
            sol::table meshPartTable = value.as<sol::table>();

            auto points_optional = meshPartTable.get<sol::optional<sol::table>>("points");
            if (!points_optional) throw "Meshpart is missing a points table";
            sol::table points = *points_optional;

            if (points.size() % 20 != 0) throw "Points table must contain a multiple of 20 values";

            // Populate the Vertices and Indices vectors from the points table
            std::vector<BlockModelVertex> vertices;
            std::vector<unsigned int> indices;

            for (int i = 1; i <= points.size() / 5; i++) {
                int offset = (i - 1) * 5 + 1;

                glm::vec3 pos(points[offset], points[offset + 1], points[offset + 2]);
                glm::vec2 tex(points[offset + 3], points[offset + 4]);

                vertices.push_back(BlockModelVertex {pos, {}, tex, tex, {}, {}});
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
            auto texture = textures[std::min(tex - 1, (int) textures.size() - 1)];

            bool blendInd = false;
            std::string blendMask = "";

            if (strncmp(texture.data(), "tint(", 5) == 0 && texture.find_last_of(')') != std::string::npos) {
                // Biome tinting time
                texture.erase(std::remove(texture.begin(), texture.end(), ' '), texture.end());

                std::string::size_type paramsBegin = texture.find_first_of('(');
                std::string::size_type paramsEnd = texture.find_last_of(')');

                std::string paramsString = texture.substr(paramsBegin + 1, paramsEnd - paramsBegin - 1);

                std::vector<std::string> params;
                std::string::size_type pos = 0;
                while ((pos = paramsString.find(',')) != std::string::npos) {
                    params.push_back(paramsString.substr(0, pos));
                    paramsString.erase(0, pos + 1);
                }
                params.push_back(paramsString);

                if (params.size() < 2) throw "Invalid biome tint values. Must have at least 2 params.";

                texture = params[1];
                blendInd = atoi(params[0].data()) + 1; //TODO: support multiple blend colors
                blendMask = (params.size() >= 3 ? params[2] : "");
            }

            // Add texture refs to blockModel if the textures table is provided
            std::shared_ptr<AtlasRef> textureRef = nullptr, blendMaskRef = nullptr;
            if (atlas) {
                textureRef = (*atlas)[texture];
                model.textureRefs.insert(textureRef);

                if (blendInd && !blendMask.empty()) {
                    blendMaskRef = (*atlas)[blendMask];
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

            Dir d = face == "top" ? Dir::TOP :
                    face == "bottom" ? Dir::BOTTOM :
                    face == "left" ? Dir::LEFT :
                    face == "right" ? Dir::RIGHT :
                    face == "front" ? Dir::FRONT :
                    face == "back" ? Dir::BACK :
                    face == "nocull" ? Dir::NO_CULL :
                    Dir::INVALID;

            if (d == Dir::INVALID) throw "face value is unrecognized";
            model.parts[static_cast<int>(d)].push_back(meshPart);
        });

        // Create the low-def block model
        BlockModel lowdefModel;
        auto ldRender = blockTable.get_or("lowdef_render", true);

        if (atlas) {
            std::vector<std::shared_ptr<AtlasRef>> refs;
            std::vector<bool> biomeTints;
            for (auto i = 0; i < lowdef_textures.size(); i++) {
                std::string texture = lowdef_textures[i];
                if (strncmp(texture.data(), "tint(", 5) == 0) {
                    texture = texture.substr(8, texture.length() - 8);
                    biomeTints.emplace_back(true);
                }
                else {
                    biomeTints.emplace_back(false);
                }
                refs.push_back((*atlas)[texture]);
            }
            lowdefModel = BlockModel::createCube(refs, biomeTints);
        }
        else {
            lowdefModel = BlockModel::createCube({}, {});
        }

        lowdefModel.culls = ldRender;
        lowdefModel.visible = ldRender;

        return {model, lowdefModel};
    }

    static void registerBlocks(sol::table source, sol::table blockModels, DefinitionAtlas& defs, TextureAtlas* atlas) {
        // Parses through all of the zepha.registered_blocks and makes BlockDefs.
        for (auto blockRef : source) {

            // Validate that the identifier and definition table exist
            std::string identifier = blockRef.first.as<std::string>();

            if (!blockRef.second || !blockRef.second.is<sol::table>())
                throw "register_block expects a table as the second parameter";
            sol::table blockTable = blockRef.second.as<sol::table>();

            // Basic Block Properties
            auto nameOpt = blockTable.get<sol::optional<std::string>>("name");
            if (!nameOpt) throw identifier + " is missing name property!";

            bool solid = blockTable.get_or("solid", true);
            auto maxStack = blockTable.get_or("stack", 64);

            // Parse through selection boxes and collision boxes
            auto selectionOpt = blockTable.get<sol::optional<sol::table>>("selection_box");
            auto collisionOpt = blockTable.get<sol::optional<sol::table>>("collision_box");

            std::vector<SelectionBox> selectionBoxes {};
            try { if (selectionOpt) selectionBoxes = parseBoxes(*selectionOpt); }
            catch (const char* error) { throw std::string("selection boxes " + std::string(error)).c_str(); }
            if (selectionBoxes.size() == 0) selectionBoxes.emplace_back(glm::vec3 {0, 0, 0}, glm::vec3 {1, 1, 1});

            std::vector<SelectionBox> collisionBoxes {};
            try { if (collisionOpt) collisionBoxes = parseBoxes(*collisionOpt); }
            catch (const char* error) { throw std::string("collision boxes " + std::string(error)).c_str(); }
            if (collisionBoxes.size() == 0) collisionBoxes.emplace_back(glm::vec3 {0, 0, 0}, glm::vec3 {1, 1, 1});

            // Create the block model
            std::pair<BlockModel, BlockModel> models = createBlockModel(blockTable, blockModels, atlas);

            BlockDef *blockDef = new BlockDef(
                    identifier,
                    defs.size(),
                    *nameOpt,
                    maxStack,
                    models.first, models.second,
                    solid,
                    std::move(selectionBoxes),
                    std::move(collisionBoxes)
            );

            // Create entity model
            if (atlas) blockDef->createModel();

            // Bind Callbacks
            auto on_place = blockTable.get<sol::optional<sol::function>>("on_place");
            if (on_place) blockDef->callbacks.insert({Callback::PLACE, *on_place});

            auto on_place_client = blockTable.get<sol::optional<sol::function>>("on_place_client");
            if (on_place_client) blockDef->callbacks.insert({Callback::PLACE_CLIENT, *on_place_client});

            auto on_break = blockTable.get<sol::optional<sol::function>>("on_break");
            if (on_break) blockDef->callbacks.insert({Callback::BREAK, *on_break});

            auto on_break_client = blockTable.get<sol::optional<sol::function>>("on_break_client");
            if (on_break_client) blockDef->callbacks.insert({Callback::BREAK_CLIENT, *on_break_client});

            auto on_construct = blockTable.get<sol::optional<sol::function>>("on_construct");
            if (on_construct) blockDef->callbacks.insert({Callback::CONSTRUCT, *on_construct});

            auto after_construct = blockTable.get<sol::optional<sol::function>>("after_construct");
            if (after_construct) blockDef->callbacks.insert({Callback::AFTER_CONSTRUCT, *after_construct});

            auto on_destruct = blockTable.get<sol::optional<sol::function>>("on_destruct");
            if (on_destruct) blockDef->callbacks.insert({Callback::DESTRUCT, *on_destruct});

            auto after_destruct = blockTable.get<sol::optional<sol::function>>("after_destruct");
            if (after_destruct) blockDef->callbacks.insert({Callback::AFTER_DESTRUCT, *after_destruct});

            // Add Block Definition to the Atlas
            defs.registerDef(blockDef);
        }
    }

    static void server(sol::table& core, ServerDefs& defs) {
        registerBlocks(core.get<sol::table>("registered_blocks"),
                core.get<sol::table>("registered_blockmodels"), defs.defs, nullptr);
    }

    static void client(sol::table& core, LocalDefs& defs) {
        registerBlocks(core.get<sol::table>("registered_blocks"),
                core.get<sol::table>("registered_blockmodels"), defs.defs, &defs.textures);
    }
};
