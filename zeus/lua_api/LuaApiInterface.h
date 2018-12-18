//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LUAAPIINTERFACE_H
#define ZEUS_LUAAPIINTERFACE_H

#include <iostream>
#include "LuaParser.h"

class LuaApiInterface {
public:
    virtual void bind_methods(LuaParser* parser) {};
};

#endif //ZEUS_LUAAPIINTERFACE_H
