//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "lua/LuaParser.h"

#include "util/CovariantPtr.h"
#include "lua/LocalModHandler.h"
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
	
	LocalModHandler& getHandler();
	
	private:
	void loadApi(WorldPtr world, PlayerPtr player);
	
	virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const override;
	
	sol::protected_function_result runFileSandboxed(const std::string& file);
	
	LuaKeybindHandler keybinds;
	LocalModHandler handler;
	double accumulatedDelta = 0;
};
