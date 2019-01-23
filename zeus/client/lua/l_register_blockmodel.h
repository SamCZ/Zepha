//
// Created by aurailus on 31/12/18.
//

#ifndef ZEUS_LREGISTERMODEL_H
#define ZEUS_LREGISTERMODEL_H

#include "LuaApi.h"

class l_register_blockmodel : LuaApi {
public:
    l_register_blockmodel(GameScene* game, LuaParser* parser);

private:
    void api(std::string identifer, sol::table data);
};


#endif //ZEUS_LREGISTERMODEL_H
