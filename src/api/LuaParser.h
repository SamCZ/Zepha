//
// Created by aurailus on 11/06/19.
// The Lua API superclass for both the client and the server.
// This class contains platform agnostic functions and structures for both of the APIs.
//
//

#ifndef ZEUS_LUAPARSER_H
#define ZEUS_LUAPARSER_H

#include <list>
#include <iostream>
#include <glm/vec3.hpp>
#include <sol2/sol.hpp>

#include "../util/Path.h"
#include "../util/Log.h"

class LuaParser {
public:
    static inline void override_panic(sol::optional<std::string> message) {
        std::cout << Log::err << "Zeus has panicked! Error:" << Log::endl;
        if (message) std::cout << Log::err << message.value() << Log::endl;
    }

    struct DelayedFunction {
        sol::function function;
        std::vector<sol::object> args;
        float timeout;
        float initial_timeout;
    };

    explicit LuaParser(std::string root);

    virtual void update();

    sol::table vecToTable(glm::vec3 vec);

    std::string root;
    Path root_path;

    sol::state lua;
    sol::table zeus;

    std::list<DelayedFunction> delayed_functions;
protected:
    int DoFileSandboxed(std::string file);
};


#endif //ZEUS_LUAPARSER_H
