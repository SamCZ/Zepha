//
// Created by aurailus on 21/04/19.
//

#include "LModuleUtil.h"

LModuleUtil::LModuleUtil(sol::state &lua, sol::table &zeus, LocalDefs &defs) {
    zeus.set_function("get_path", [&](std::string modname) {
        try {
            if (modname.length() == 0) throw "No mod name specified!";
            return "/home/aurailus/C++/GlProject/res/lua/" + modname + "/";
        }
        catch (const std::string& e) {
            std::cerr << "Exception on get_path: " << e << std::endl;
        }
    });
}
