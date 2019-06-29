//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_LOCALREGISTERBLOCKS_H
#define ZEUS_LOCALREGISTERBLOCKS_H


#include <sol.hpp>
#include "../../def/LocalDefs.h"

class LocalRegisterBlocks {
public:
    LocalRegisterBlocks(sol::table& zeus, LocalDefs& defs);
};


#endif //ZEUS_LOCALREGISTERBLOCKS_H
