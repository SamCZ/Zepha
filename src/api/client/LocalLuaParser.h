//
// Created by aurailus on 17/12/18.
//

#ifndef ZEUS_LOCALLUAPARSER_H
#define ZEUS_LOCALLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files.h>
#include <list>

class LocalDefs;
class LocalWorld;

class LocalLuaParser : public LuaParser {
public:
    explicit LocalLuaParser(std::string mod_root);

    void init(LocalDefs& defs, LocalWorld& world);
    void update();

    struct DelayedFunction {
        sol::function function;
        std::vector<sol::object> args;
        float timeout;
        float initial_timeout;
    };
    std::list<DelayedFunction> delayed_functions;

    ~LocalLuaParser() = default;
};


#endif //ZEUS_LOCALLUAPARSER_H