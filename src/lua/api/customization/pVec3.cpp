//
// Created by aurailus on 31/08/19.
//

#include "pVec3.h"

bool sol_lua_check(sol::types<glm::vec3>, lua_State* L, int index, sol_handler_function handler, sol::stack::record& tracking) {
    int absolute_index = lua_absindex(L, index);

    return sol::stack::check<sol::table>(L, index, handler, tracking);
}

glm::vec3 sol_lua_get(sol::types<glm::vec3>, lua_State* L, int index, sol::stack::record& tracking) {
    sol::table vec3table = sol::stack::get<sol::table>(L, index, tracking);
    float x = vec3table["x"];
    float y = vec3table["y"];
    float z = vec3table["z"];
    return glm::vec3{ x, y, z };
}

int sol_lua_push(sol::types<glm::vec3>, lua_State* L, const glm::vec3& vec) {
    sol::state_view lua(L);
    sol::table vec3table = sol::table::create_with(L, "x", vec.x, "y", vec.y, "z", vec.z);
    int amount = sol::stack::push(L, vec3table);
    return amount;
}