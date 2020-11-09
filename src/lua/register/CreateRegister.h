/*
 * A Utility Function that creates register functions for Lua.
 *
 * - Auri, 03/11/20
 */

#pragma once

#include <string>
#include <functional>
#include <sol/forward.hpp>

namespace Api::Util {
    void createRegister(sol::state& lua, sol::table& core, const std::string& name,
        std::function<void(std::string)> after = nullptr, const std::string& table = "");
}