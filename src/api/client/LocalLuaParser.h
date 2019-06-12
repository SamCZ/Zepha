//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LOCALLUAPARSER_H
#define ZEUS_LOCALLUAPARSER_H

#include <string>
#include <iostream>
#include <cute_files.h>
#include "../LuaParser.h"

#include "ModuleClientRegisterBlock.h"
#include "ModuleClientUtils.h"

class LocalDefs;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(std::string mod_root);

    void init(LocalDefs& defs);

    ~LocalLuaParser() = default;
};


#endif //ZEUS_LOCALLUAPARSER_H