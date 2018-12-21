//
// Created by aurailus on 18/12/18.
//

#ifndef ZEUS_LREGISTERBLOCK_H
#define ZEUS_LREGISTERBLOCK_H

#include "../world/GameInstance.h"

class LRegisterBlock {
public:
    explicit LRegisterBlock(GameInstance* game);
    void regApi(sol::table* Z);
private:
    void api(std::string identifer, sol::table data);
    GameInstance* game;
};


#endif //ZEUS_LREGISTERBLOCK_H
