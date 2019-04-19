//
// Created by aurailus on 18/12/18.
//

#include "LModuleRegister.h"
#include "../../def/GameDefs.h"

LModuleRegister::LModuleRegister(sol::state &lua, sol::table &zeus, GameDefs &defs) {
    auto registered_blockmodels = lua.create_table();
    zeus["registered_blockmodels"] = registered_blockmodels;

    zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
        if (identifier.length() == 0) {
            std::cerr << "Tried to initialize blockmodel without providing an Identifier." << std::endl;
            return;
        }

        zeus["registered_blockmodels"][identifier] = data;
    });

    zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
        if (identifier.length() == 0) {
            printf("Tried to initialize block without identifier!\n");
            return;
        }

        auto name = data.get<sol::optional<std::string>>("name");
        auto texTable = data.get<sol::optional<sol::table>>("textures");
        auto selectionBoxTable = data.get<sol::optional<sol::table>>("selection_box");
        auto modelName = data.get_or<std::string>("model", "default:cube");

        bool visible = data.get_or("visible", true);
        bool culls = data.get_or("culls", true);
        bool solid = data.get_or("solid", true);

        if (!name || !texTable) {
            printf("Tried to initialize block without data.");
            return;
        }

        sol::table models = zeus["registered_blockmodels"];
        auto modelOptional = models.get<sol::optional<sol::table>>(modelName);

        if (!modelOptional) {
            printf("Tried to initialize using invalid model %s\n", modelName.c_str());
            return;
        }

        SelectionBox selectionBox;
        if (!selectionBoxTable) {
            selectionBox = {{0, 0, 0}, {1, 1, 1}};
        }
        else {
            sol::table box = (*selectionBoxTable)[1];

            selectionBox = {{box[1], box[2], box[3]}, {box[4], box[5], box[6]}};
        }


        BlockModel* model = BlockModel::from_lua_def(*modelOptional, *texTable, defs.textures(), visible, culls);

        BlockDef* def = new BlockDef(identifier, model, solid, selectionBox);

        defs.blocks().registerBlock(def);
    });
}