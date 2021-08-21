//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "lua/LuaParser.h"

#include "lua/LuaMod.h"
#include "client/gui/Root.h"
#include "client/gui/Element.h"
#include "client/gui/GuiBuilder.h"

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
	
	void loadAndRunMod(const string& modPath);
	
	void showError(const string& err);
	
	sol::protected_function_result runFileSandboxed(const string& file);
	
	virtual sol::protected_function_result errorCallback(sol::protected_function_result r) override;
	
	LuaMod mod {};
	string subgameName;
	
	Client& client;
	
	Gui::Root& root;
	sptr<Gui::Element> sandboxRoot;
	vec<sptr<AtlasRef>> menuAssets {};
	
};
