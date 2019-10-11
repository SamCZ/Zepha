//
// Created by aurailus on 28/06/19.
//

#include "LocalRegisterBlocks.h"

LocalRegisterBlocks::LocalRegisterBlocks(sol::table& core, LocalDefs &defs) {
    //Register all of the blocks in the zepha.registered_blocks table.
    for (auto blockRef : core.get<sol::table>("registered_blocks")) {
        //Get the unique identifier of the block
        std::string identifier = blockRef.first.as<std::string>();

        //Make sure the block table is actually a table, and get it.
        if (!blockRef.second || !blockRef.second.is<sol::table>())
            throw identifier + "'s definition table is not a table!";
        sol::table blockTable = blockRef.second.as<sol::table>();

        //Get block properties and throw errors if required ones are missing
        auto nameOpt      = blockTable.get<sol::optional<std::string>>("name");
        auto modelStrOpt  = blockTable.get<sol::optional<std::string>>("model");
        auto texturesOpt  = blockTable.get<sol::optional<sol::table>> ("textures");
        auto selectionOpt = blockTable.get<sol::optional<sol::table>> ("selection_box");
        auto ldTexturesOpt= blockTable.get<sol::optional<sol::table>> ("lowdef_textures");

        if (!nameOpt) throw identifier + " is missing name property!";
        if (!texturesOpt) throw identifier + " is missing textures property!";

        bool visible = blockTable.get_or("visible", true);
        bool culls   = blockTable.get_or("culls", true);
        bool solid   = blockTable.get_or("solid", true);
        auto ldRender= blockTable.get_or("lowdef_render", true);

        //Get the identifier for the blockModel, and then get the model from the zepha.registered_blockmodels table.
        std::string modelStr = (modelStrOpt ? *modelStrOpt : "default:cube");
        sol::optional<sol::table> modelOpt = core.get<sol::table>("registered_blockmodels").get<sol::table>(modelStr);
        if (!modelOpt) throw identifier + " specifies invalid model " + modelStr + "!";

        //Create a vector of selection boxes
        std::vector<SelectionBox> sBoxes;
        if (selectionOpt) {
            for (auto pair : *selectionOpt) {
                sol::table s = pair.second;
                sBoxes.push_back({{s[1], s[2], s[3]}, {s[4], s[5], s[6]}});
            }
        } else {
            sBoxes.push_back({{0, 0, 0}, {1, 1, 1}});
        }

        //Create a block model from the above properties
        sol::table model = *modelOpt;
        BlockModel blockModel;

        blockModel.culls = culls;
        blockModel.visible = visible;

        //Convert Textures Table to Vector
        std::vector<std::string> textures;
        for (auto pair : *texturesOpt) {
            if (!pair.second.is<std::string>()) throw "Textures table has non-string value!";
            textures.push_back(pair.second.as<std::string>());
        }
        if (textures.size() == 0) textures.push_back("_missing");

        //Convert LowDef Textures Table to Vector
        std::vector<std::string> lowdef_textures;
        if (ldTexturesOpt) {
            for (auto pair : *ldTexturesOpt) {
                if (!pair.second.is<std::string>()) throw "Textures table has non-string value!";
                lowdef_textures.push_back(pair.second.as<std::string>());
            }
            if (lowdef_textures.size() == 0) lowdef_textures.push_back("_missing");
        }
        else {
            lowdef_textures = textures;
        }

        //Add Mesh Mods
        sol::optional<sol::table> meshModTable = model.get<sol::optional<sol::table>>("mesh_mods");
        if (meshModTable) {
            for (auto modEntry : (*meshModTable)) {
                auto modTable = modEntry.second.as<sol::table>();
                std::string meshMod = modTable.get_or<std::string>("type", "none");

                if (meshMod == "none") { continue; }
                else if (meshMod == "offset_x") {
                    blockModel.meshMods.emplace_back(
                            MeshMod::OFFSET_X, modTable.get_or<float>("amplitude", 1));
                }
                else if (meshMod == "offset_y") {
                    blockModel.meshMods.emplace_back(
                            MeshMod::OFFSET_Y, modTable.get_or<float>("amplitude", 1));
                }
                else if (meshMod == "offset_z") {
                    blockModel.meshMods.emplace_back(
                            MeshMod::OFFSET_Z, modTable.get_or<float>("amplitude", 1));
                }
            }
        }

        //Convert all of the mesh parts to C++ Objects and add them to blockModel
        model.get<sol::table>("parts").for_each([&](sol::object key, sol::object value) {

            //Make sure LocalMeshPart is in fact a table
            if (!value.is<sol::table>()) throw "Meshpart is not a table";
            sol::table meshPartTable = value.as<sol::table>();

            //Get The points table, and make sure it's valid
            auto points_optional = meshPartTable.get<sol::optional<sol::table>>("points");
            if (!points_optional) throw "Meshpart is missing a points table (Local)";

            sol::table points = *points_optional;

            if (points.size() % 20 != 0) throw "Points array is ill-formed. (Not a multiple of 20 values)";

            //Populate the Vertices and Indices vectors from the points table
            std::vector<BlockModelVertex> vertices;
            std::vector<unsigned int> indices;

            for (int i = 1; i <= points.size()/5; i++) {
                int offset = (i - 1) * 5 + 1;

                glm::vec3 pos(points[offset], points[offset + 1], points[offset + 2]);
                glm::vec2 tex(points[offset + 3], points[offset + 4]);

                vertices.push_back({pos, {0, 0, 0}, tex, {0, 0}});
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

            //Get the texture for the part
            int tex = std::max((int)meshPartTable.get_or<float>("tex", 1), 1);
            auto texture = textures[std::min(tex - 1, (int)textures.size() - 1)];
            auto textureRef = defs.textures().getTextureRef(texture);

            //Add a reference to the texture to the blockModel's list of required textures to keep it in memory.
            blockModel.textureRefs.insert(textureRef);

            //Create a LocalMeshPart object
            MeshPart meshPart(std::move(vertices), std::move(indices), textureRef);

            //Add ShaderMod
            sol::optional<sol::table> shaderModTable = meshPartTable.get<sol::optional<sol::table>>("shader_mod");
            if (shaderModTable) {
                std::string shaderMod = (*shaderModTable).get_or<std::string>("type", "none");

                if (shaderMod == "none") {
                    meshPart.shaderMod = ShaderMod::NONE;
                }
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

            //Add the meshpart to the proper face vector
            std::string face = meshPartTable.get_or<std::string>("face", "nocull");

            Dir d = face == "top"    ? TOP     :
                    face == "bottom" ? BOTTOM  :
                    face == "left"   ? LEFT    :
                    face == "right"  ? RIGHT   :
                    face == "front"  ? FRONT   :
                    face == "back"   ? BACK    :
                    face == "nocull" ? NO_CULL :
                    INVALID;

            if (d == INVALID)
                throw "Face value \"" + face + "\" is not one of 'top', 'bottom', 'left', 'right', 'front', 'back', 'nocull'.";

            blockModel.parts[d].push_back(meshPart);
        });

        std::vector<std::shared_ptr<AtlasRef>> refs;
        for (auto i = 0; i < lowdef_textures.size(); i++) {
            refs.push_back(defs.textures().getTextureRef(lowdef_textures[i]));
        }
        BlockModel lowdefBlockModel = BlockModel::createCube(refs);
        lowdefBlockModel.culls = ldRender;
        lowdefBlockModel.visible = ldRender;

        //TODO: Update the selection boxes thingy
        BlockDef* blockDef = new BlockDef(identifier, defs.defs().size(), *nameOpt, blockModel, solid, std::move(sBoxes));
        blockDef->createModel();

        //Bind Callbacks
        auto on_place_client = blockTable.get<sol::optional<sol::function>>("on_place_client");
        if (on_place_client) blockDef->callbacks.insert({Callback::PLACE_CLIENT, *on_place_client});

        auto on_break_client = blockTable.get<sol::optional<sol::function>>("on_break_client");
        if (on_break_client) blockDef->callbacks.insert({Callback::BREAK_CLIENT, *on_break_client});

        //Add Block Definition to the Atlas
        defs.defs().registerDef(blockDef);
    }
}
