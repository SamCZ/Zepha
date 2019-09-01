//
// Created by aurailus on 31/08/19.
//

#pragma once

#include <sol2/sol.hpp>
#include <glm/vec3.hpp>

using sol_handler_function = std::function<void(lua_State*, int, sol::type, sol::type, const char*)>;

bool sol_lua_check(sol::types<glm::vec3>, lua_State* L, int index, sol_handler_function handler, sol::stack::record& tracking);
glm::vec3 sol_lua_get(sol::types<glm::vec3>, lua_State* L, int index, sol::stack::record& tracking);
int sol_lua_push(sol::types<glm::vec3>, lua_State* L, const glm::vec3& vec);
