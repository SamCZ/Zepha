//
// Created by aurailus on 18/12/18.
//

#include "LModuleRegister.h"
#include "../../def/GameDefs.h"

LModuleRegister::LModuleRegister(sol::state &lua, sol::table &zeus, GameDefs &defs) {

    //Define registered_blockmodels table
    zeus["registered_blockmodels"] = lua.create_table();

    //
    // # Register BlockModel
    // `zeus.register_blockmodel(string identifier, table definition)`
    //
    // Definition is stored as `zeus.registered_blockmodels[identifier] = definition`.
    // There are no requirements for the contents of the definition table in this function, but for it to be well
    // formed and usable in `register_block`, it should be an array-table of face tables, which contain the following
    // information:
    //
    // face:    One of "left", "right", "front", "back", "top", "bottom", or "nocull". Used when determining which parts
    //          of the model to cull.
    //
    // tex:     The texture index which this face uses. What textures these refer to are defined in the `register_block`
    //          `textures` table. The index must be greater than or equal to 1.
    //
    // points:  An array of integers that define one or more quads. Each quad is defined by 20 numbers or 4 vertices,
    //          in the format of x, y, z, tex_coord_x, tex_coord_y. The vertices are indexed in the order
    //          0, 1, 2, 2, 3, 0.
    //
    // ## Example
    // {
    //     face = "left"
    //     tex = 3,
    //     points = {
    //         0, 0, 0, 0, 1,
    //         0, 0, 1, 1, 1,
    //         0, 1, 1, 1, 0,
    //         0, 1, 0, 0, 0
    //     }
    // }
    //

    zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
        try {
            if (identifier.length() == 0) throw "No Identifier";
            zeus["registered_blockmodels"][identifier] = data;
        }
        catch (const std::string& e) {
            std::cerr << "Exception on register_blockmodel: " << e << std::endl;
        }
    });

    //
    // # Register Block
    // `zeus.register_block(string identifier, table definition)`
    //
    //
    //
    zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
        try {
            if (identifier.length() == 0) throw "No Identifier";

            auto name       = data.get<sol::optional<std::string>>("name");
            auto textures   = data.get<sol::optional<sol::table>>("textures");
            auto modelStr   = data.get_or<std::string>("model", "default:cube");
            auto selection  = data.get<sol::optional<sol::table>>("selection_box");

            bool visible    = data.get_or("visible", true);
            bool culls      = data.get_or("culls", true);
            bool solid      = data.get_or("solid", true);

            if (!name) throw "No name property";
            if (!textures) throw "No textures table";

            //Convert textures to a std::vector
            std::vector<std::string> texturesVector;
            textures->for_each([&](sol::object key, sol::object value) {
                if (!value.is<std::string>()) throw "Non string texture value.";
                std::string str = value.as<std::string>();
                texturesVector.push_back(str);
            });
            if (texturesVector.size() == 0) texturesVector.push_back("_missing");

            //TODO: In register_blockmodel, store these models in C++ somewhere so that I don't need to reference the
            //lua data to register a model.
            sol::table models = zeus["registered_blockmodels"];
            auto model = models.get<sol::optional<sol::table>>(modelStr);

            if (!model) throw "Undefined model";

            //TODO: Validate and allow multiple selection boxes
            SelectionBox sbox = {{0, 0, 0}, {1, 1, 1}};
            if (selection) {
                sol::table def = (*selection)[1];
                sbox = {{def[1], def[2], def[3]}, {def[4], def[5], def[6]}};
            }

            BlockModel blockModel = BlockModel::create(*model, texturesVector, defs.textures(), visible, culls);
            BlockDef def(identifier, std::move(blockModel), solid, sbox);

            defs.blocks().registerBlock(std::move(def));
        }
        catch (const std::string& e) {
            std::cerr << "Exception on register_block: " << e << std::endl;
        }
    });
}