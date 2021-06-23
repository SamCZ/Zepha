//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "lua/LuaParser.h"

#include "lua/LuaMod.h"
#include "client/gui/GuiBuilder.h"

class Client;

class Subgame;

class AtlasRef;

class SubgameDef;

class GuiContainer;

class MenuSandbox : LuaParser {
	public:
	MenuSandbox(glm::ivec2& window, Client& client, std::shared_ptr<GuiContainer> container);
	
	void load(const SubgameDef& subgame);
	
	void update(double delta) override;
	
	void windowResized();
	
	using LuaParser::update;
	private:
	void reset();
	
	void loadApi();
	
	void loadAndRunMod(const std::string& modPath);
	
	void showError(const std::string& what, const std::string& subgame);
	
	sol::protected_function_result runFileSandboxed(const std::string& file);
	
	virtual sol::protected_function_result errorCallback(sol::protected_function_result r) const override;
	
	LuaMod mod {};
	std::vector<std::shared_ptr<AtlasRef>> modAssets{};
	
	std::shared_ptr<GuiContainer> container = nullptr;
	std::shared_ptr<GuiContainer> luaContainer = nullptr;
	GuiBuilder builder;
	
	Client& client;
	glm::ivec2& win;
};
