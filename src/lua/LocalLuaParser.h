//
// Created by aurailus on 17/12/18.
//

#pragma once

#include <unordered_map>

#include "lua/LuaParser.h"

#include "LuaMod.h"
#include "util/CovariantPtr.h"
#include "lua/LuaKeybindHandler.h"

class Client;
class LocalWorld;
class LocalPlayer;
class LocalSubgame;

class LocalLuaParser : public LuaParser {
public:
	explicit LocalLuaParser(LocalSubgame& game);
	
	void init(WorldPtr world, PlayerPtr player, Client& client);
	
	void update(double delta) override;
	
	void addMod(const LuaMod& mod);
	
	void setModLoadOrder(const vec<string> order);
	
private:
	void loadApi(WorldPtr world, PlayerPtr player);
	
	virtual sol::protected_function_result errorCallback(sol::protected_function_result r) override;
	
	sol::protected_function_result runFileSandboxed(const std::string& file);
	
	Client* client;
	LuaKeybindHandler keybinds;
	double accumulatedDelta = 0;
	
	std::unordered_map<string, LuaMod> mods {};
	vec<string> modLoadOrder {};
};
