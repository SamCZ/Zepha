//
// Created by aurailus on 11/06/19.
//

#include "LuaParser.h"

void LuaParser::update(double delta) {
    // Execute delayed functions
    // TODO: Experiment with storing delayed functions inside of Lua and test performance.

    auto it = delayed_functions.begin();
    while (it != delayed_functions.end()) {
        DelayedFunction& f = *it;
        f.timeout -= delta;
        if (f.timeout <= 0) {
            if (f.function(sol::as_args(f.args))) {
                f.timeout = f.initial_timeout;
            } else {
                it = delayed_functions.erase(it);
                continue;
            }
        }
        it++;
    }
}

sol::table LuaParser::luaVec(glm::vec3 vec) {
    return lua["vector"]["new"](vec.x, vec.y, vec.z);
}

sol::table LuaParser::luaVec(sol::state_view s, glm::vec3 vec) {
    return s["vector"]["new"](vec.x, vec.y, vec.z);
}
