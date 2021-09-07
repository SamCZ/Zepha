//
// Created by aurailus on 17/12/18.
//

#pragma once

#include <unordered_set>
#include <unordered_map>

#include "lua/LuaParser.h"

#include "Mod.h"
#include "client/Callback.h"
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
	
	void addMod(const Mod& mod);
	
	void setModLoadOrder(const vec<string> order);
	
	void addKBObserver(usize id);
	
	void removeKBObserver(usize id);
	
private:
	void loadApi(WorldPtr world);
	
	/** Exposed to Lua, can provide a relative path, uses environment variables to resolve. */
	sol::protected_function_result require(sol::this_environment env, string requirePath);
	
	/** Loads a file with the right enviroment, needs a canonical path. */
	sol::protected_function_result loadFile(string path);
	
	Client* client;
	PlayerPtr player;
	double accumulatedDelta = 0;
	
	vec<CallbackRef> refs;
	vec<string> modOrder {};
	std::unordered_set<usize> kbObservers {};
	std::unordered_map<string, Mod> mods {};
};
