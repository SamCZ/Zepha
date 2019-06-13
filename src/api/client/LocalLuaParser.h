//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LOCALLUAPARSER_H
#define ZEUS_LOCALLUAPARSER_H

#include <string>
#include <iostream>
#include <cute_files.h>
#include "../LuaParser.h"

class LocalDefs;
class LocalWorld;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(std::string mod_root);

    void init(LocalDefs& defs, LocalWorld& world);

    ~LocalLuaParser() = default;
};


#endif //ZEUS_LOCALLUAPARSER_H