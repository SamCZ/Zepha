//
// Created by aurailus on 18/12/18.
//

#include "l_register_block.h"

//Add a block to the scene.
//The first variable is the identifer, which is used internally and by mods to reference the block.
//The second variable is a table containing all the block's data. This is used to define properties of the nodeblock.
void l_register_block::api(std::string identifier, sol::table data) {

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

    sol::table models = (*parser->getModule())["registered_blockmodels"];
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


    BlockModel* model = BlockModel::from_lua_def(*modelOptional, *texTable, *game->textures, visible, culls);

    BlockDef* def = new BlockDef(identifier, model, solid, selectionBox);

    game->blocks->registerBlock(def);
}

l_register_block::l_register_block(GameScene *game, LuaParser *parser) : LuaApi(game, parser) {
    parser->getModule()->set_function("register_block", &l_register_block::api, this);
}
