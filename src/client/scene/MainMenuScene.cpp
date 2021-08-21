#include <fstream>
#include <iostream>
#include <json/json.hpp>
#include <cute_files/cute_files.h>

#include "MainMenuScene.h"

#include "util/Log.h"
#include "ConnectScene.h"
#include "client/Client.h"
#include "client/gui/BoxElement.h"
#include "client/gui/TextElement.h"
#include "client/menu/SubgameDef.h"
#include "client/gui/basic/GuiText.h"
#include "game/atlas/asset/AtlasRef.h"
#include "client/gui/basic/GuiContainer.h"
#include "client/gui/compound/GuiImageButton.h"

MainMenuScene::MainMenuScene(Client& client) : Scene(client),
	root(client.renderer.window, client.game->textures),
	sandboxElem(root.create<Gui::BoxElement>({ .classes = { "sandbox" }})),
	sandbox(client, root, sandboxElem) {
	
	client.renderer.setClearColor(0, 0, 0);
	client.renderer.window.input.setMouseLocked(false);
	
	using Expr = Gui::Expression;

	root.addStylesheet({
		{ "navigation", {{
			{ Gui::StyleRule::SIZE, array<Expr, 2> { Expr("-1"), Expr("18dp") } }
		}}},
		{ "navigationWrap", {{
			{ Gui::StyleRule::DIRECTION, string("row") },
			{ Gui::StyleRule::POS, array<Expr, 2> { Expr("0"), Expr("0") } }
		}}},
		{ "navigationBackground", {{
			{ Gui::StyleRule::SIZE, array<Expr, 2> { Expr("64dp"), Expr("18dp") } },
			{ Gui::StyleRule::BACKGROUND, string("menu_bar_bg") }
		}}},
		{ "navigationButton", {{
			{ Gui::StyleRule::SIZE, array<Expr, 2> { Expr("16dp"), Expr("16dp") } },
			{ Gui::StyleRule::CURSOR, string("pointer") }
		}}}
	});

	root.body->append(sandboxElem);
	let navigation = root.body->append<Gui::BoxElement>({ .classes = { "navigation" } });
	let navigationBG = navigation->append<Gui::BoxElement>({ .classes = { "navigationWrap" } });

	for (usize i = 0; i < 2000 / Gui::PX_SCALE / 64; i++)
		navigationBG->append<Gui::BoxElement>({ .classes = { "navigationBackground" } });

	let navigationList = navigation->append<Gui::BoxElement>({
		.classes = { "navigationWrap" },
		.styles = {{
			{ Gui::StyleRule::PADDING, array<Expr, 4> { Expr("1dp"), Expr("1dp"), Expr("1dp"), Expr("1dp") } },
			{ Gui::StyleRule::GAP, array<Expr, 2> { Expr("1dp"), Expr("1dp") } }
		}}
	});

	let serversButton = navigationList->append<Gui::BoxElement>({
		.classes = { "navigationButton" },
		.styles = {{
			{ Gui::StyleRule::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_multiplayer)") },
			{ Gui::StyleRule::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_multiplayer)") }
		}}
	});

	let contentButton = navigationList->append<Gui::BoxElement>({
		.classes = { "navigationButton" },
		.styles = {{
			{ Gui::StyleRule::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_content)") },
			{ Gui::StyleRule::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_content)") }
		}}
	});

	navigationList->append<Gui::BoxElement>({
		.styles = {{
			{ Gui::StyleRule::BACKGROUND, string("#fff5") },
			{ Gui::StyleRule::SIZE, array<Expr, 2> { Expr("1dp"), Expr("10dp") } },
			{ Gui::StyleRule::MARGIN, array<Expr, 4> { Expr("2dp"), Expr("3dp"), Expr("2dp"), Expr("3dp") } }
		}}
	});

	findSubgames();

	for (usize i = 0; i < subgames.size(); i++) {
		let& subgame = subgames[i];
		
		let elem = navigationList->append<Gui::BoxElement>({
			.classes = { "navigationButton" },
			.styles = {{
				{ Gui::StyleRule::BACKGROUND, string("crop(0, 0, 16, 16, " + subgame.iconRef->name + ")") },
				{ Gui::StyleRule::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, " + subgame.iconRef->name + ")") }
			}}
		});
		
		elem->onClick([&](u32 button, bool down) {
			if (button != GLFW_MOUSE_BUTTON_1) return;
			selectedSubgame = &subgame;
			sandbox.load(*selectedSubgame);
		});
	}

	if (subgames.size() > 0) {
		selectedSubgame = &subgames[0];
		sandbox.load(*selectedSubgame);
	}

	navigationList->append<Gui::BoxElement>();

	let settingsButton = navigationList->append<Gui::BoxElement>({
		.classes = { "navigationButton" },
		.styles = {{
			{ Gui::StyleRule::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_settings)") },
			{ Gui::StyleRule::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_settings)") }
		}}
	});

	let closeButton = navigationList->append<Gui::BoxElement>({
		.classes = { "navigationButton" },
		.styles = {{
			{ Gui::StyleRule::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_quit)") },
			{ Gui::StyleRule::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_quit)") }
		}}
	});

//		closeButton->setCallback(Element::CallbackType::PRIMARY,
//			[](bool down, ivec2) { if (down) exit(0); });

//		contentButton->setCallback(Element::CallbackType::PRIMARY, [&](bool down, ivec2) {
//			if (!down) return;
//			client.scene.setScene(make_unique<ConnectScene>(client, Address{ "127.0.0.1" }));
//		});
}

void MainMenuScene::findSubgames() {
	string subgamesPath = "../subgames";
	
	cf_dir_t subgamesDir;
	cf_dir_open(&subgamesDir, subgamesPath.data());
	while (subgamesDir.has_next) {
		cf_file_t subgameFolder;
		cf_read_file(&subgamesDir, &subgameFolder);
		if (!subgameFolder.is_dir || strncmp(subgameFolder.name, ".", 1) == 0) {
			cf_dir_next(&subgamesDir);
			continue;
		}
		
		try {
			bool hasConf = false, hasIcon = false, hasMods = false;
			
			cf_dir_t subgame;
			cf_dir_open(&subgame, subgameFolder.path);
			while (subgame.has_next) {
				cf_file_t file;
				cf_read_file(&subgame, &file);
				
				if (!file.is_dir && strncmp(file.name, "icon.png\0", 9) == 0) hasIcon = true;
				if (!file.is_dir && strncmp(file.name, "conf.json\0", 10) == 0) hasConf = true;
				if (file.is_dir && strncmp(file.name, "mods\0", 5) == 0) hasMods = true;
				
				cf_dir_next(&subgame);
			}
			cf_dir_close(&subgame);
			
			if (!hasConf)
				throw std::runtime_error(
					string("Subgame ") + string(subgameFolder.name) + " is missing a conf.json.");
			if (!hasMods)
				throw std::runtime_error(
					string("Subgame ") + string(subgameFolder.name) + " is missing a 'mods' directory.");
			
			nlohmann::json j{};
			try {
				std::ifstream(string(subgameFolder.path) + "/conf.json") >> j;
			}
			catch (...) {
				throw std::runtime_error(string(subgameFolder.name) + "/conf.json is not a valid JSON object.");
			}
			
			if (!j.is_object())
				throw std::runtime_error(string(subgameFolder.name) + "/conf.json is not a valid JSON object.");
			if (!j["name"].is_string() || j["name"] == "")
				throw std::runtime_error(
					"The 'name' property in " + string(subgameFolder.name) + "/conf.json is missing or invalid.");
			if (!j["version"].is_string() || j["version"] == "")
				throw std::runtime_error("The 'version' property in " + string(subgameFolder.name) +
				                         "/conf.json is missing or invalid.");
			
			string name = j["name"];
			string description = (j["description"].is_string() ? j["description"] : "");
			string version = j["version"];
			
			std::shared_ptr<AtlasRef> icon = client.game->textures["menu_flag_missing"];
			if (hasIcon) icon = client.game->textures.loadImage(string(subgameFolder.path) + "/icon.png", name);
			
			subgames.push_back({ icon, { name, description, version }, subgameFolder.path });
		}
		catch (const std::runtime_error& e) {
			std::cout << Log::err << "Encountered an error while loading subgames:\n\t" << e.what() << Log::endl;
		}
		
		cf_dir_next(&subgamesDir);
	}
	cf_dir_close(&subgamesDir);
	
	std::sort(subgames.begin(), subgames.end(),
		[](SubgameDef& a, SubgameDef& b) { return a.config.name < b.config.name; });
}

void MainMenuScene::update() {
	client.game->textures.update();
	root.update();
//	sandbox.update(client.getDelta());
	
//	components->handleMouseInput(client.renderer.window);
}

void MainMenuScene::draw() {
	let& renderer = client.renderer;
	
	renderer.beginChunkDeferredCalls();
	renderer.endDeferredCalls();
	
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	root.draw(renderer);
}