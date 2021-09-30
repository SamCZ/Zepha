#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "MainMenuScene.h"

#include "util/Log.h"
#include "ConnectScene.h"
#include "client/Client.h"
#include "client/gui/BoxElement.h"
#include "client/gui/TextElement.h"
#include "client/menu/SubgameDef.h"
#include "client/gui/basic/GuiText.h"
#include "game/atlas/asset/AtlasTexture.h"
#include "client/gui/basic/GuiContainer.h"
#include "client/gui/compound/GuiImageButton.h"

MainMenuScene::MainMenuScene(Client& client) : Scene(client),
	root(client.renderer.window, client.game->textures),
	sandboxElem(root.create<Gui::BoxElement>()),
	sandbox(client, root, sandboxElem) {
	
	client.renderer.setClearColor(0, 0, 0);
	client.renderer.window.input.setMouseLocked(false);
	
	using Expr = Gui::Expression;

	root.addStylesheet({
		{ "navigation", {{
			{ Gui::Prop::SIZE, array<Expr, 2> { Expr(""), Expr("18dp") } }
		}}},
		{ "navigationWrap", {{
			{ Gui::Prop::DIRECTION, string("row") },
			{ Gui::Prop::POS, array<Expr, 2> { Expr("0"), Expr("0") } }
		}}},
		{ "navigationBackground", {{
			{ Gui::Prop::SIZE, array<Expr, 2> { Expr("64dp"), Expr("18dp") } },
			{ Gui::Prop::BACKGROUND, string("menu_bar_bg") }
		}}},
		{ "navigationButton", {{
			{ Gui::Prop::SIZE, array<Expr, 2> { Expr("16dp"), Expr("16dp") } },
			{ Gui::Prop::CURSOR, string("pointer") }
		}}}
	});

	root.body->append(sandboxElem);
	let navigation = root.body->append<Gui::BoxElement>({{{ Gui::Prop::CLASS, vec<string> { "navigation" } }}});
	let navigationBG = navigation->append<Gui::BoxElement>({{{ Gui::Prop::CLASS, vec<string> { "navigationWrap" } }}});

	for (usize i = 0; i < 2000 / Gui::PX_SCALE / 64; i++)
		navigationBG->append<Gui::BoxElement>({{{ Gui::Prop::CLASS, vec<string> { "navigationBackground" } }}});

	let navigationList = navigation->append<Gui::BoxElement>({{
		{ Gui::Prop::CLASS, vec<string> { "navigationWrap" } },
		{ Gui::Prop::GAP, array<Expr, 2> { Expr("1dp"), Expr("1dp") } },
		{ Gui::Prop::PADDING, array<Expr, 4> { Expr("1dp"), Expr("1dp"), Expr("1dp"), Expr("1dp") } }
	}});

	navigationList->append<Gui::BoxElement>({{
		{ Gui::Prop::CLASS, vec<string> { "navigationButton" } },
		{ Gui::Prop::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_multiplayer)") },
		{ Gui::Prop::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_multiplayer)") }
	}});

	let contentButton = navigationList->append<Gui::BoxElement>({{
		{ Gui::Prop::CLASS, vec<string> { "navigationButton" } },
		{ Gui::Prop::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_content)") },
		{ Gui::Prop::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_content)") }
	}});

	contentButton->onClick([&](u32 button, bool down) {
		if (button != GLFW_MOUSE_BUTTON_1 || !down) return;
		client.scene.setScene(make_unique<ConnectScene>(client, Address { "127.0.0.1" }));
	});
	
	navigationList->append<Gui::BoxElement>({{
		{ Gui::Prop::BACKGROUND, string("#fff5") },
		{ Gui::Prop::SIZE, array<Expr, 2> { Expr("1dp"), Expr("10dp") } },
		{ Gui::Prop::MARGIN, array<Expr, 4> { Expr("2dp"), Expr("3dp"), Expr("2dp"), Expr("3dp") } }
	}});
	
	let subgamesPath = std::filesystem::path("..") / "subgames";
	std::filesystem::directory_iterator iter(subgamesPath);
	
	for (let& file : iter) {
		if (!file.is_directory()) continue;
		
		try {
			if (!std::filesystem::exists(file.path() / "conf.json")) throw std::runtime_error("Missing a conf.json");
			if (!std::filesystem::exists(file.path() / "mods")) throw std::runtime_error("Missing a mods folder");
			
			nlohmann::json json {};
			try {
				std::ifstream(file.path() / "conf.json") >> json;
			}
			catch (...) {
				throw std::runtime_error("conf.json has a syntax error");
			}
			
			if (!json.is_object())
				throw std::runtime_error("conf.json has a syntax error.");
			if (!json["name"].is_string() || json["name"] == "")
				throw std::runtime_error("conf.json is missing 'name'");
			if (!json["version"].is_string() || json["version"] == "")
				throw std::runtime_error("conf.json is missing 'version'");
			
			const AtlasTexture icon = std::filesystem::exists(file.path() / "icon.png")
				? client.game->textures.addFile((file.path() / "icon.png").string(), false)
				: client.game->textures["menu_flag_missing"];
			
			subgames.push_back({ icon, { json["name"], json["description"], json["version"] }, file.path() });
		}
		catch(const std::runtime_error& e) {
			std::cout << Log::err << "Failed to load subgame '"
			          << file.path().filename().string() << "', " << e.what() << "." << std::endl;
		}
	}
	
	std::sort(subgames.begin(), subgames.end(),
		[](SubgameDef& a, SubgameDef& b) { return a.config.name < b.config.name; });

	for (usize i = 0; i < subgames.size(); i++) {
		let& subgame = subgames[i];
		
		let elem = navigationList->append<Gui::BoxElement>({{
			{ Gui::Prop::CLASS, vec<string> { "navigationButton" } },
			{ Gui::Prop::BACKGROUND, string("crop(0, 0, 16, 16, " + subgame.iconRef.getIdentifier() + ")") },
			{ Gui::Prop::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, " + subgame.iconRef.getIdentifier() + ")") }
		}});
		
		elem->onClick([&](u32 button, bool down) {
			if (button != GLFW_MOUSE_BUTTON_1 || !down) return;
			selectedSubgame = &subgame;
			sandbox.load(*selectedSubgame);
		});
	}

	if (subgames.size() > 0) {
		selectedSubgame = &subgames[0];
		sandbox.load(*selectedSubgame);
	}

	navigationList->append<Gui::BoxElement>();

	navigationList->append<Gui::BoxElement>({{
		{ Gui::Prop::CLASS, vec<string> { "navigationButton" } },
		{ Gui::Prop::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_settings)") },
		{ Gui::Prop::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_settings)") }
	}});

	let closeButton = navigationList->append<Gui::BoxElement>({{
		{ Gui::Prop::CLASS, vec<string> { "navigationButton" } },
		{ Gui::Prop::BACKGROUND, string("crop(0, 0, 16, 16, menu_flag_quit)") },
		{ Gui::Prop::BACKGROUND_HOVER, string("crop(16, 0, 16, 16, menu_flag_quit)") }
	}});

	closeButton->onClick([](u32 button, bool down) {
		if (button != GLFW_MOUSE_BUTTON_1 || !down) return;
		exit(0);
	});
}

void MainMenuScene::update() {
	root.update();
	sandbox.update(client.getDelta());
}

void MainMenuScene::draw() {
	let& renderer = client.renderer;
	
	renderer.beginChunkDeferredCalls();
	renderer.endDeferredCalls();
	
	renderer.enableTexture(client.game->textures.getTexture());
	renderer.beginGUIDrawCalls();
	root.draw(renderer);
}