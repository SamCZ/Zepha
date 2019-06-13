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
        std::cout << Log::err << "Error opening \"" + file + "\", access denied." << Log::endl;
    }
}

sol::table LuaParser::vecToTable(glm::vec3 vec) {
    return lua.create_table_with("x", vec.x, "y", vec.y, "z", vec.z);
}
