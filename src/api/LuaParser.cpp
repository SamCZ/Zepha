//
// Created by aurailus on 11/06/19.
//

#include "LuaParser.h"

LuaParser::LuaParser(std::string root) :
        root(std::move(root)),
        root_path(this->root) {}

int LuaParser::DoFileSandboxed(std::string file) {
    if (root_path.contains(Path(file))) {
        lua.script_file(file);
    }
    else {
        std::cerr << "\"" + file + "\" is outside of the sandbox and cannot be loaded!" << std::endl;
    }
}
