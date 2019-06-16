//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_SERVERLUAPARSER_H
#define ZEUS_SERVERLUAPARSER_H

#include "../LuaParser.h"
#include <cute_files.h>
#include <list>

class ServerDefs;
class ServerWorld;

class ServerLuaParser : public LuaParser {
public:
    explicit ServerLuaParser(std::string mod_root);

    void init(ServerDefs& defs, ServerWorld& world);
    void update();

    struct DelayedFunction {
        sol::function function;
        std::vector<sol::object> args;
        float timeout;
        float initial_timeout;
    };
    std::list<DelayedFunction> delayed_functions;

    ~ServerLuaParser() = default;
};


#endif //ZEUS_SERVERLUAPARSER_H
