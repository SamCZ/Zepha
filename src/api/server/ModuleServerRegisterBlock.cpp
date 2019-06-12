//
// Created by aurailus on 11/06/19.
//

#include "ModuleServerRegisterBlock.h"
#include "../../def/ServerDefs.h"

ModuleServerRegisterBlock::ModuleServerRegisterBlock(sol::state &lua, sol::table &zeus, ServerDefs &defs) {

    //Define registered_blockmodels table
    zeus["registered_blockmodels"] = lua.create_table();

    zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
        try {
            if (identifier.length() == 0) throw "No Identifier";
            zeus["registered_blockmodels"][identifier] = data;
        }
        catch (const std::string& e) {
            std::cout << Log::err << "Exception on register_blockmodel: " << e << Log::endl;
        }
    });

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

            ServerBlockModel blockModel = ServerBlockModel::create(*model, texturesVector, visible, culls);
            ServerBlockDef def(identifier, defs.blocks().definitionsSize(), std::move(blockModel), solid, sbox);

            defs.blocks().registerBlock(std::move(def));
        }
        catch (const std::string& e) {
            std::cout << Log::err << "Exception on register_block: " << e << Log::endl;
        }
    });
}