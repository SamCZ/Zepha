//
// Created by aurailus on 18/12/18.
//

#include "l_register_block.h"

//Add a block to the game.
//The first variable is the identifer, which is used internally and by mods to reference the block.
//The second variable is a table containing all the block's data. This is used to define properties of the nodeblock.
void l_register_block::api(std::string identifier, sol::table data) {

    if (identifier.length() == 0) {
        printf("Tried to initialize block without identifier!\n");
        return;
    }

    auto name = data.get<sol::optional<std::string>>("name");
    auto texTable = data.get<sol::optional<sol::table>>("textures");
    auto modelName = data.get_or<std::string>("model", "default:cube");

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

    BlockModel* model = BlockModel::from_lua_def(*modelOptional, *texTable, game->textureAtlas);

    BlockDef* def = new BlockDef(identifier, model);

    game->blockAtlas->registerBlock(def);
}

l_register_block::l_register_block(GameInstance *game, LuaParser *parser) : LuaApi(game, parser) {
    parser->getModule()->set_function("register_block", &l_register_block::api, this);
}
