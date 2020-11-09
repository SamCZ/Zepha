//
// Created by aurailus on 11/06/19.
//

#include "LuaParser.h"

void LuaParser::bindModules() {
	for (auto& module : modules) module->bind();
}
