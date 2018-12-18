//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_AVERAGE_H
#define ZEUS_AVERAGE_H


#include "../LuaApiInterface.h"

class l_Average : public LuaApiInterface {
public:
    void bind_methods(LuaParser* parser) override;
};


#endif //ZEUS_AVERAGE_H
