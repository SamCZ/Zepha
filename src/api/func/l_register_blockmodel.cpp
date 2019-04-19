//
// Created by aurailus on 31/12/18.
//

#include "l_register_blockmodel.h"

void l_register_blockmodel::api(std::string identifier, sol::table data) {
    if (identifier.length() == 0) {
        printf("Tried to initialize block without identifier!\n");
        return;
    }

    (*parser->getModule())["registered_blockmodels"][identifier] = data;
}

l_register_blockmodel::l_register_blockmodel(GameScene* game, LuaParser* parser) : LuaApi(game, parser) {

    auto registered_blockmodels = parser->getState()->create_table();
    (*parser->getModule())["registered_blockmodels"] = registered_blockmodels;

    parser->getModule()->set_function("register_blockmodel", &l_register_blockmodel::api, this);
}
