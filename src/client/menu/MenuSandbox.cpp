
#include <fstream>
#include <iostream>
#include <cute_files/cute_files.h>

#include "MenuSandbox.h"

#include "lua/LuaMod.h"
#include "client/Client.h"
#include "lua/ErrorFormatter.h"
#include "client/menu/SubgameDef.h"
#include "client/gui/basic/GuiText.h"
#include "client/gui/basic/GuiContainer.h"

// Modules
#include "lua/modules/Time.h"
#include "lua/modules/mSetGui.h"
#include "lua/modules/mStartGame.h"

MenuSandbox::MenuSandbox(glm::ivec2& win, Client& client, std::shared_ptr<GuiContainer> container) :
	LuaParser(*client.game),
	win(win),
	client(client)
//	container(container),
//	luaContainer(std::dynamic_pointer_cast<GuiContainer>(container->add(std::make_shared<GuiContainer>("__lua"))))
//	builder(client.game->textures, client.game->models, luaContainer) {}
{}

void MenuSandbox::reset() {
//	container->remove("error");
//	builder.clear(true);
	core = {};
	mod = {};
	lua = sol::state{};
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi();
}

void MenuSandbox::loadApi() {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	
	modules.emplace_back(std::make_unique<Api::Module::Time>(Api::State::CLIENT, lua, core));
	
//	ClientApi::gui_element(lua);
	
//	MenuApi::set_gui(builder, win, lua, core);
	MenuApi::start_game(client, core);
	
	bindModules();
	
	// Create sandboxed runfile()
	lua["dofile"] = lua["loadfile"] = sol::nil;
	lua.set_function("runfile", &MenuSandbox::runFileSandboxed, this);
}

void MenuSandbox::load(const SubgameDef& subgame) {
	reset();
	
	try {
		loadAndRunMod(subgame.subgamePath + "/../../assets/base");
		loadAndRunMod(subgame.subgamePath + "/menu");
	}
	catch (const std::runtime_error& e) {
		showError(e.what(), subgame.config.name);
	}
}

void MenuSandbox::windowResized() {
//	builder.build(win);
}

void MenuSandbox::update(double delta) {
//	builder.update();
	core["__builtin"]["update_delayed_functions"]();
}

sol::protected_function_result MenuSandbox::runFileSandboxed(const std::string& file) {
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

void MenuSandbox::loadAndRunMod(const std::string& modPath) {
	if (!cf_file_exists(modPath.data())) throw std::runtime_error("Directory not found.");
	
	LuaMod mod;
	std::string root = modPath + "/script";
	
	std::list<std::string> dirsToScan{ root };
	std::list<std::string> luaFiles{};
	
	cf_dir_t dir;
	while (!dirsToScan.empty()) {
		std::string dirStr = *dirsToScan.begin();
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
	
	for (std::string& file : luaFiles) {
		size_t rootPos = file.find(root);
		std::string modPath = file;
		
		if (rootPos == std::string::npos)
			throw std::runtime_error("Attempted to access file \"" + file + "\", which is outside of the mod root.");
		
		modPath.erase(rootPos, root.length() + 1);
		modPath.resize(modPath.size() - 4);
		
		std::ifstream t(file);
		std::string fileStr((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
		
		LuaMod::File f{ modPath, fileStr };
		mod.files.push_back(f);
	}
	
	std::string texPath = modPath + "/textures";
	if (cf_file_exists(texPath.data())) {
		this->modAssets = client.game->textures.loadDirectory(texPath, false, true);
	}
	
	this->mod = mod;
	runFileSandboxed("init");
}

void MenuSandbox::showError(const std::string& what, const std::string& subgame) {
	const std::string errPrefixText = "Encountered an error while loading the menu for " + subgame + " ;-;";
//	Font f(client.game->textures, client.game->textures["font"]);
	
//	auto errWrap = std::make_shared<GuiContainer>("error");
//	container->add(errWrap);
	
//	auto errPrefix = std::make_shared<GuiText>("error_text");
//	errPrefix->create({ 3, 3 }, {}, { 0.7, 0, 0.3, 1 }, { 1, 1, 1, 1 }, f);
//	errPrefix->setText(errPrefixText);
//	errPrefix->setPos({ 8, 16 });
//	errWrap->add(errPrefix);
	
//	auto errMsg = std::make_shared<GuiText>("error_text");
//	errMsg->create({ 3, 3 }, {}, {}, { 1, 0.5, 0.6, 1 }, f);
//	errMsg->setText(what);
//	errMsg->setPos({ 8, 52 });
//	errWrap->add(errMsg);
}

sol::protected_function_result MenuSandbox::errorCallback(sol::protected_function_result r) const {
	sol::error err = r;
	std::string errString = err.what();
	
	try {
		std::string::size_type lineNumStart = errString.find(':');
		if (lineNumStart == std::string::npos) throw std::out_of_range("Improperly formatted error. [0]");
		std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
		if (lineNumEnd == std::string::npos) throw std::out_of_range("Improperly formatted error. [1]");
		
		std::string fileName = errString.substr(0, lineNumStart);
		int lineNum = std::stoi(errString.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));
		
		for (const LuaMod::File& file : mod.files)
			if (file.path == fileName)
				throw std::runtime_error(ErrorFormatter::formatError(fileName, lineNum, errString, file.file));
		
		throw std::out_of_range("Error thrown outside of handled files. [2]");
	}
	catch (const std::runtime_error& e) {
		std::cout << Log::err << e.what() << std::endl;
		throw;
	}
	catch (const std::out_of_range& e) {
		std::cout << Log::err << "Failed to format error, " << e.what() << Log::endl;
		throw std::runtime_error(errString);
	}
}
