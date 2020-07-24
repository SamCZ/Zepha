//
// Vec3 to Vector table customization.
// Allows sol to transfer glm::vec3 and glm::vec3 to a Lua vector table and back.
// This customization was based off of code provided by rubenwardy.
// Created by aurailus on 2020-07-22.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include <glm/vec3.hpp>
#include "../../util/Util.h"

namespace sol {

    // Float

    template <>
    struct lua_size<glm::vec3> : std::integral_constant<int, 1> {};

    template <>
    struct lua_type_of<glm::vec3> : std::integral_constant<type, type::table> {};

    template <typename Handler>
    inline bool sol_lua_check(types<glm::vec3>, lua_State* L, int index, Handler&& handler, stack::record& tracking) {
        int absoluteIndex = lua_absindex(L, index);
        if (!stack::check<table>(L, absoluteIndex, handler)) {
            tracking.use(1);
            return false;
        }

        stack::get_field(L, 1, absoluteIndex);
        bool x = stack::check<float>(L, -1);

        stack::get_field(L, 2, absoluteIndex);
        bool y = stack::check<float>(L, -1);

        stack::get_field(L, 3, absoluteIndex);
        bool z = stack::check<float>(L, -1);

        stack::pop_n(L, 3);
        tracking.use(1);
        return x && y && z;
    }

    inline glm::vec3 sol_lua_get(types<glm::vec3>, lua_State* L, int index, stack::record& tracking) {
        int absoluteIndex = lua_absindex(L, index);

        sol::table table = stack::get<sol::table>(L, absoluteIndex);
        float x = table[1];
        float y = table[2];
        float z = table[3];

        tracking.use(1);
        return glm::vec3 { x, y, z };
    }


    inline int sol_lua_push(sol::types<glm::vec3>, lua_State* L, const glm::vec3& vec) {
        sol::state_view lua(L);

        lua_createtable(L, 3, 0);
        sol::stack_table tbl(L, -1);
        tbl[1] = vec.x; tbl[2] = vec.y; tbl[3] = vec.z;
        tbl[sol::metatable_key] = lua["vector"];

        return 1;
    }

    // Float

    template <>
    struct lua_size<glm::ivec3> : std::integral_constant<int, 1> {};

    template <>
    struct lua_type_of<glm::ivec3> : std::integral_constant<type, type::table> {};

    template <typename Handler>
    inline bool sol_lua_check(types<glm::ivec3>, lua_State* L, int index, Handler&& handler, stack::record& tracking) {
        int absoluteIndex = lua_absindex(L, index);
        if (!stack::check<table>(L, absoluteIndex, handler)) {
            tracking.use(1);
            return false;
        }

        stack::get_field(L, 1, absoluteIndex);
        bool x = stack::check<int>(L, -1);

        stack::get_field(L, 2, absoluteIndex);
        bool y = stack::check<int>(L, -1);

        stack::get_field(L, 3, absoluteIndex);
        bool z = stack::check<int>(L, -1);

        stack::pop_n(L, 3);
        tracking.use(1);
        return x && y && z;
    }

    inline glm::ivec3 sol_lua_get(types<glm::ivec3>, lua_State* L, int index, stack::record& tracking) {
        int absoluteIndex = lua_absindex(L, index);

        sol::table table = stack::get<sol::table>(L, absoluteIndex);
        int x = table[1];
        int y = table[2];
        int z = table[3];

        tracking.use(1);
        return glm::ivec3 { x, y, z };
    }


    inline int sol_lua_push(sol::types<glm::ivec3>, lua_State* L, const glm::ivec3& vec) {
        sol::state_view lua(L);

        lua_createtable(L, 3, 0);
        sol::stack_table tbl(L, -1);
        tbl[1] = vec.x; tbl[2] = vec.y; tbl[3] = vec.z;
        tbl[sol::metatable_key] = lua["vector"];

        return 1;
    }

}

#pragma clang diagnostic pop