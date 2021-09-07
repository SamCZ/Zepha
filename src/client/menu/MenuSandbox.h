//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "lua/LuaParser.h"

#include "lua/Mod.h"
#include "client/gui/Root.h"
#include "client/gui/Element.h"

class Client;
class Subgame;
class AtlasRef;
class SubgameDef;
class GuiContainer;

class MenuSandbox : LuaParser {
public:
	MenuSandbox(Client& client, Gui::Root& root, sptr<Gui::Element> sandboxRoot);
	
	void load(const SubgameDef& subgame);
	
	void update(double delta) override;
	
	using LuaParser::update;

private:
	void reset();
	
	void loadApi();
	
	void loadMod(const std::filesystem::path& path);
	
	void showError(const string& err);
	
	/** Exposed to Lua, can provide a relative path, uses environment variables to resolve. */
	sol::protected_function_result require(sol::this_environment thisEnv, const string& path);
	
	/** Loads a file with the right enviroment, needs a canonical path. */
	sol::protected_function_result loadFile(const string& path);
	
	Mod mod {};
	string subgameName;
	
	Client& client;
	
	Gui::Root& root;
	sptr<Gui::Element> sandboxRoot;
	vec<sptr<AtlasRef>> menuAssets {};
	
	double accumulatedDelta = 0;
	
};
