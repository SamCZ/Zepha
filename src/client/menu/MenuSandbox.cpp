#include <fstream>
#include <iostream>
#include <client/gui/TextElement.h>

#include "MenuSandbox.h"

#include "lua/Mod.h"
#include "client/Client.h"
#include "lua/ErrorFormatter.h"
#include "client/menu/SubgameDef.h"

// Modules
#include "lua/modules/Time.h"
#include "lua/modules/mSetGui.h"
#include "lua/modules/mStartGame.h"
#include "lua/usertype/GuiElement.h"

MenuSandbox::MenuSandbox(Client& client, Gui::Root& root, sptr<Gui::Element> sandboxRoot) :
	LuaParser(*client.game),
	client(client),
	root(root),
	sandboxRoot(sandboxRoot) {}

void MenuSandbox::reset() {
	sandboxRoot->clear();
	core = {};
	mod = {};
	lua = sol::state {};
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi();
}

void MenuSandbox::loadApi() {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	modules.emplace_back(std::make_unique<Api::Module::Time>(Api::State::CLIENT, lua, core));
	
	Api::Usertype::GuiElement::bind(lua, core, root);
	MenuApi::set_gui(lua, core, sandboxRoot);
	MenuApi::start_game(client, core);
	
	bindModules();
	
	lua.set_function("require", &MenuSandbox::require, this);
	lua["dofile"] = lua["loadfile"] = lua["require"];
}

void MenuSandbox::load(const SubgameDef& subgame) {
	reset();
	subgameName = subgame.config.name;

	try {
		loadMod(subgame.subgamePath / "../../assets/base");
		loadMod(subgame.subgamePath / "menu");
	}
	catch (sol::error e) {
		string err = static_cast<sol::error>(e).what();
	
		vec<string> lines;
		{
			string line;
			std::stringstream textStream(err);
			while (std::getline(textStream, line, '\n')) lines.emplace_back(line);
		}

		for (const let& line : lines) {
			usize lineNumStart = line.find(':');
			if (lineNumStart == string::npos) continue;
			usize lineNumEnd = line.find(':', lineNumStart + 1);
			if (lineNumEnd == string::npos) continue;
		
			string fileName = line.substr(0, lineNumStart);
			fileName.erase(std::remove_if(fileName.begin(), fileName.end(), isspace), fileName.end());
			
			for (const let& file : mod.files) {
				if (file.first != fileName) continue;
			
				let msg = ErrorFormatter::formatError(fileName,
					std::stoi(line.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1)),
					err, file.first);
			
				showError(msg);
				return;
			}
		}
	
		showError(err);
	}
}

void MenuSandbox::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

sol::protected_function_result MenuSandbox::require(sol::this_environment thisEnv, const string& path) {
	sol::environment env = static_cast<sol::environment>(thisEnv);
	string currentPath = env.get<string>("__PATH");
	
	std::cout << "CURRENT: " << currentPath << "\nPATH: " << path << std::endl;

	return loadFile(path);
	//	auto modName = env.get<std::string>("_MODNAME");
//	std::string iden = identifier[0] == ':' ? modName + identifier : identifier;
}

sol::protected_function_result MenuSandbox::loadFile(const string& path) {
	let it = mod.files.find(path);
	if (it == mod.files.end()) throw sol::error("Error opening '" + path + "', file not found.");
	let& file = it->second;
	
	sol::environment env(lua, sol::create, lua.globals());
	env["_PATH"] = path.substr(0, path.find_last_of('/') + 1);
	env["_FILE"] = path;
	env["_MODNAME"] = mod.name;
	
	using Pfr = sol::protected_function_result;
	return lua.safe_script(file, env,
		[](lua_State*, Pfr pfr) -> Pfr { throw static_cast<sol::error>(pfr); },
		"@" + path, sol::load_mode::text);
}

void MenuSandbox::loadMod(const std::filesystem::path& path) {
	mod = { path, true };
	
	if (std::filesystem::exists(path / "textures"))
		menuAssets = client.game->textures.addDirectory((path / "textures"), false);
	
	loadFile("/main");
}

void MenuSandbox::showError(const string& err) {
	const string errPrefixText = "`cfEncountered an error while loading the menu for '" + subgameName + "' ;-;\n\n`r";

	using Expr = Gui::Expression;
	sandboxRoot->clear();
	sandboxRoot->append<Gui::TextElement>({{
		{ Gui::Prop::CONTENT, errPrefixText + err },
		{ Gui::Prop::TEXT_SIZE, Expr("2px") },
		{ Gui::Prop::SIZE, array<Expr, 2> { Expr("100dp"), Expr("-1") } },
		{ Gui::Prop::MARGIN, array<Expr, 4> { Expr("4dp"), Expr("4dp"), Expr("4dp"), Expr("4dp") } }
	}});
}