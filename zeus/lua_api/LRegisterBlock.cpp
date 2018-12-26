//
// Created by aurailus on 18/12/18.
//

#include "LRegisterBlock.h"

LRegisterBlock::LRegisterBlock(GameInstance *game) {
    this->game = game;
}

void LRegisterBlock::regApi(sol::table *Z) {
    Z->set_function("register_block", &LRegisterBlock::api, this);
}

//Api Function
void LRegisterBlock::api(std::string identifier, sol::table data) {
    if (identifier.length() == 0) {
        printf("Tried to initialize block without identifier!\n");
        return;
    }

    auto name = data.get<sol::optional<std::string>>("name");
    auto texTable = data.get<sol::optional<sol::table>>("textures");

    if (name && texTable) {
//        printf("Registering block %s!\n", name->c_str());

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

        return;
    }
    else {
        printf("Tried to initialize block without required arguments!\n");
    }
}
