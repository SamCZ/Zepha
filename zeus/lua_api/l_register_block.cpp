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

    auto textures = std::vector<std::string>();
    int size = texTable->size();

    //LUA is 1-indexed
    for (int i = 1; i <= 6; i++) {
        //Get either the texture for this index or the last texture.
        //This is done so you can implicitly specify how your block is textured like so:
        //1 Texture: all sides take this texture
        //3 Textures: top and bottom have seperate textures, all side faces share a third texture
        //6 Textures: each face has it's own texture.

        auto texture = texTable->get<std::string>(min(i, size));
        textures.push_back(texture);
    }

    BlockModel* model = BlockModel::Square(textures[0].c_str(), textures[1].c_str(), textures[2].c_str(),
            textures[3].c_str(), textures[4].c_str(), textures[5].c_str(), game->textureAtlas);

    BlockDef* def = new BlockDef(identifier, model);

    game->blockAtlas->registerBlock(def);
}

l_register_block::l_register_block(GameInstance *game, LuaParser *parser) : LuaApi(game, parser) {
    parser->getModule()->set_function("register_block", &l_register_block::api, this);
}
