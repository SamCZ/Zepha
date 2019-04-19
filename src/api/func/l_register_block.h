//
// Created by aurailus on 18/12/18.
//

#ifndef ZEUS_LREGISTERBLOCK_H
#define ZEUS_LREGISTERBLOCK_H

#include "../LuaApi.h"
#include "../../def/block/graph/SelectionBox.h"

class l_register_block : LuaApi {
public:
    l_register_block(GameScene* game, LuaParser* parser);

private:
    void api(std::string identifer, sol::table data);
};


#endif //ZEUS_LREGISTERBLOCK_H
