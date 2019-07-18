//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_SGETPATH_H
#define ZEUS_SGETPATH_H

#include <sol2/sol.hpp>
#include "../../../util/Log.h"

namespace ServerApi {
    void get_path(sol::table& zeus, const std::string& lua_path) {
        zeus.set_function("get_path", [&](std::string modname) {
            if (modname.length() <= 0) throw "get_path must supply a mod name!";
            return lua_path + modname + "/";
        });
    }
}

#endif //ZEUS_CGETPATH_H
