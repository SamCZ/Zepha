#include <fstream>
#include <iostream>
#include <cute_files.h>
#include <client/gui/TextElement.h>

#include "MenuSandbox.h"

#include "lua/LuaMod.h"
#include "client/Client.h"
#include "lua/ErrorFormatter.h"
#include "client/menu/SubgameDef.h"

// Modules
#include "lua/modules/Time.h"
#include "lua/modules/mSetGui.h"
#include "lua/modules/mStartGame.h"
#include "lua/usertype/LuaGuiElement.h"

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
//	ClientApi::gui_element(lua);
	
	MenuApi::set_gui(lua, core, sandboxRoot);
	MenuApi::start_game(client, core);
	
	bindModules();
	
	// Create sandboxed runfile()
	lua["dofile"] = lua["loadfile"] = sol::nil;
	lua.set_function("runfile", &MenuSandbox::runFileSandboxed, this);
}

void MenuSandbox::load(const SubgameDef& subgame) {
	reset();
	subgameName = subgame.config.name;

	loadAndRunMod(subgame.subgamePath + "/../../assets/base");
	loadAndRunMod(subgame.subgamePath + "/menu");
}

void MenuSandbox::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

sol::protected_function_result MenuSandbox::runFileSandboxed(const string& file) {
	for (LuaMod::File& f : mod.files) {
		if (f.path != file) continue;
		
		sol::environment env(lua, sol::create, lua.globals());
		env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
		env["_FILE"] = f.path;
		env["_MODNAME"] = mod.config.name;
		
		return lua.safe_script(f.file, env, std::bind(&MenuSandbox::errorCallback,
			this, std::placeholders::_2), "@" + f.path, sol::load_mode::text);
	}
	throw std::runtime_error("Error opening '" + file + "', file not found.");
}

void MenuSandbox::loadAndRunMod(const string& modPath) {
	if (!cf_file_exists(modPath.data())) throw std::runtime_error("Directory not found.");
	
	LuaMod mod;
	string root = modPath + "/script";
	
	std::list<string> dirsToScan{ root };
	std::list<string> luaFiles{};
	
	cf_dir_t dir;
	while (!dirsToScan.empty()) {
		string dirStr = *dirsToScan.begin();
		dirsToScan.erase(dirsToScan.begin());
		
		if (!cf_file_exists(dirStr.data())) throw std::runtime_error("Missing 'script' directory.");
		cf_dir_open(&dir, dirStr.data());
		
		while (dir.has_next) {
			// Read through files in the directory
			cf_file_t scannedFile;
			cf_read_file(&dir, &scannedFile);
			
			if (strncmp(scannedFile.name, ".", 1) != 0) {
				if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
				else {
					auto path = std::string_view(scannedFile.path);
					if (path.rfind('.') != std::string::npos && path.rfind('.') == path.rfind(".lua"))
						luaFiles.emplace_back(scannedFile.path);
				}
			}
			cf_dir_next(&dir);
		}
		cf_dir_close(&dir);
	}
	
	mod.modPath = modPath;
	
	for (string& file : luaFiles) {
		size_t rootPos = file.find(root);
		string modPath = file;
		
		if (rootPos == std::string::npos)
			throw std::runtime_error("Attempted to access file \"" + file + "\", which is outside of the mod root.");
		
		modPath.erase(rootPos, root.length() + 1);
		modPath.resize(modPath.size() - 4);
		
		std::ifstream t(file);
		string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		
		LuaMod::File f { modPath, fileStr };
		mod.files.push_back(f);
	}
	
	string texPath = modPath + "/textures";
	if (cf_file_exists(texPath.data()))
		menuAssets = client.game->textures.loadDirectory(texPath, false, true);
	
	this->mod = mod;
	runFileSandboxed("init");
}

void MenuSandbox::showError(const string& err) {
	const string errPrefixText = "`cfEncountered an error while loading the menu for '" + subgameName + "' ;-;\n\n`r";

	using Expr = Gui::Expression;
	sandboxRoot->clear();
	sandboxRoot->append<Gui::TextElement>({
		.styles = {{
			{ Gui::StyleRule::CONTENT, errPrefixText + err },
			{ Gui::StyleRule::TEXT_SIZE, Expr("2px") },
			{ Gui::StyleRule::SIZE, array<Expr, 2> { Expr("100dp"), Expr("-1") } },
			{ Gui::StyleRule::MARGIN, array<Expr, 4> { Expr("4dp"), Expr("4dp"), Expr("4dp"), Expr("4dp") } }
		}}
	});
}

sol::protected_function_result MenuSandbox::errorCallback(sol::protected_function_result r) {
	string err = static_cast<sol::error>(r).what();
	
	std::cout << Log::err << err << Log::endl;
	
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
			if (file.path == fileName) {
				let msg = ErrorFormatter::formatError(fileName,
					std::stoi(line.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1)),
					err, file.file);
				
				showError(msg);
				return r;
			}
		}
	}
	
	showError(err);
	return r;
}
