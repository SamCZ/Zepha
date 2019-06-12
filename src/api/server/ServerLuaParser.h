//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_SERVERLUAPARSER_H
#define ZEUS_SERVERLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files.h>

#include "ModuleServerRegisterBlock.h"
#include "ModuleServerUtils.h"

class ServerDefs;

class ServerLuaParser : public LuaParser {
public:
    explicit ServerLuaParser(std::string mod_root);

    void init(ServerDefs& defs);

    ~ServerLuaParser() = default;
};


#endif //ZEUS_SERVERLUAPARSER_H
