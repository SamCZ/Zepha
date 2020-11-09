//
// A simple class that stores Lua callbacks for keybinds. Has a callback function (keybindHandler)
// that is given to the Input object and called when a keyboard key changes state.
// Created by aurailus on 14/10/19.
//

#pragma once

#include "Lua.h"

class LocalLuaParser;

class LuaKeybindHandler {
	public:
	LuaKeybindHandler(LocalLuaParser* parser);
	
	LuaKeybindHandler(const LuaKeybindHandler& o) = delete;
	
	void bindOnDown(unsigned short key, const sol::protected_function& cb);
	
	void bindOnUp(unsigned short key, const sol::protected_function& cb);
	
	void keybindHandler(bool state, int i);
	
	private:
	std::array<std::vector<sol::protected_function>, 1024> callbacksDown{};
	std::array<std::vector<sol::protected_function>, 1024> callbacksUp{};
	
	const LocalLuaParser* parser;
};