#include <fstream>
#include <iostream>
#include <json/json.hpp>
#include <cute_files/cute_files.h>

#include "MainMenuScene.h"

#include "util/Log.h"
#include "ConnectScene.h"
#include "client/Client.h"
#include "client/graph/Renderer.h"
#include "client/menu/SubgameDef.h"
#include "client/gui/basic/GuiText.h"
#include "game/atlas/asset/AtlasRef.h"
#include "client/gui/basic/GuiContainer.h"
#include "client/gui/compound/GuiImageButton.h"

MainMenuScene::MainMenuScene(Client& client) :
	Scene(client),
	components(make_unique<GuiContainer>()),
	menuContainer(make_shared<GuiContainer>("__menu")),
	sandbox(sandboxArea, client, menuContainer) {
	
	client.renderer.setClearColor(0, 0, 0);
	client.renderer.window.input.lockMouse(false);
	
	Font f(client.game->textures, client.game->textures["font"]);
	win = client.renderer.window.getSize();
	sandboxArea = win - ivec2(0, 18 * GS);
	
	components->add(menuContainer);
	
	branding = make_shared<GuiContainer>("zephaBranding");
	components->add(branding);
	{
		auto zephaText = make_shared<GuiText>("zephaText");
		zephaText->create({ GS, GS }, {}, {}, { 1, 1, 1, 1 }, f);
		zephaText->setText("Zepha");
		branding->add(zephaText);
		
		auto alphaText = make_shared<GuiText>("alphaText");
		alphaText->create({ GS, GS }, {}, {}, { 1, 0.5, 0.7, 1 }, f);
		alphaText->setText("ALPHA");
		alphaText->setPos({ 25 * GS, 0 });
		branding->add(alphaText);
	}
	
	navigationBar = make_shared<GuiContainer>("navigationBar");
	navigationBar->add(make_shared<GuiContainer>("navigationBarBg"));
	navigationBar->add(make_shared<GuiContainer>("navigationBarIcons"));
	
	auto navigationBarIcons = navigationBar->get<GuiContainer>("navigationBarIcons");
	
	components->add(navigationBar);
	{
		auto settingsButton = make_shared<GuiImageButton>("settingsButton");
		settingsButton->create({ 16 * GS, 16 * GS }, {},
			client.game->textures["crop(0, 0, 16, 16, menu_flag_settings)"],
			client.game->textures["crop(16, 0, 16, 16, menu_flag_settings)"]);
		
		navigationBar->get<GuiContainer>("navigationBarIcons")->add(settingsButton);
		
		auto closeButton = make_shared<GuiImageButton>("closeButton");
		closeButton->create({ 16 * GS, 16 * GS }, {},
			client.game->textures["crop(0, 0, 16, 16, menu_flag_quit)"],
			client.game->textures["crop(16, 0, 16, 16, menu_flag_quit)"]);
		
		closeButton->setCallback(GuiComponent::CallbackType::PRIMARY,
			[](bool down, ivec2) { if (down) exit(0); });
		navigationBar->get<GuiContainer>("navigationBarIcons")->add(closeButton);
		
		auto serversButton = make_shared<GuiImageButton>("serversButton");
		serversButton->create({ 16 * GS, 16 * GS }, {},
			client.game->textures["crop(0, 0, 16, 16, menu_flag_multiplayer)"],
			client.game->textures["crop(16, 0, 16, 16, menu_flag_multiplayer)"]);
		
		serversButton->setPos({ GS, GS });
		navigationBarIcons->add(serversButton);
		
		auto contentButton = make_shared<GuiImageButton>("contentButton");
		contentButton->create({ 16 * GS, 16 * GS }, {},
			client.game->textures["crop(0, 0, 16, 16, menu_flag_content)"],
			client.game->textures["crop(16, 0, 16, 16, menu_flag_content)"]);
		
		contentButton->setPos({ GS + GS * 18, GS });
		contentButton->setCallback(GuiComponent::CallbackType::PRIMARY, [&](bool down, ivec2) {
			if (!down) return;
			client.scene.setScene(make_unique<ConnectScene>(client, Address{ "127.0.0.1" }));
		});
		
		navigationBarIcons->add(contentButton);
		
		auto divider = make_shared<GuiRect>("divider");
		divider->create({ GS, GS * 10 }, {}, { 1, 1, 1, 0.3 });
		divider->setPos({ GS * 2 + GS * 18 * 2, GS * 4 });
		navigationBarIcons->add(divider);
		
		findSubgames();
		
		for (usize i = 0; i < subgames.size(); i++) {
			auto& subgame = subgames[i];
			auto button = make_shared<GuiImageButton>(subgame.config.name);
			
			button->create({ 16 * GS, 16 * GS }, {},
				client.game->textures["crop(0, 0, 16, 16, " + subgame.iconRef->name + ")"],
				client.game->textures["crop(16, 0, 16, 16, " + subgame.iconRef->name + ")"]);
			
			button->setPos({ GS * 7 + GS * 18 * (i + 2), GS });
			button->setCallback(GuiComponent::CallbackType::PRIMARY, [&](bool down, ivec2) {
				if (!down) return;
				selectedSubgame = &subgame;
				sandbox.load(*selectedSubgame);
			});
			
			navigationBarIcons->add(button);
		}
	}
	
	if (subgames.size() > 0) {
		selectedSubgame = &subgames[0];
		sandbox.load(*selectedSubgame);
	}
	
	positionElements();
	
	client.renderer.window.addResizeCallback("mainmenu", [&](ivec2 newWin) {
		win = newWin;
		sandboxArea = newWin - ivec2(0, 18 * GS);
		positionElements();
	});
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

void MainMenuScene::positionElements() {
	sandbox.windowResized();
	
	branding->setPos({ win.x - 55 * GS, win.y - 30 * GS });
	
	navigationBar->setPos({ 0, win.y - 18 * GS });
	
	auto navigationBarBg = navigationBar->get<GuiContainer>("navigationBarBg");
	for (usize i = 0; i < static_cast<f32>(win.x) / 64.f / GS; i++) {
		auto segment = make_shared<GuiRect>("segment_" + std::to_string(i));
		segment->create({ 64 * GS, 18 * GS }, {}, client.game->textures["menu_bar_bg"]);
		segment->setPos({ i * 64 * GS, 0 });
		navigationBarBg->add(segment);
	}
	
	auto navigationBarIcons = navigationBar->get<GuiContainer>("navigationBarIcons");
	navigationBarIcons->get<GuiImageButton>("closeButton")->setPos({ win.x - 16 * GS - GS, GS });
	navigationBarIcons->get<GuiImageButton>("settingsButton")->setPos({ win.x - 16 * GS * 2 - GS * 3, GS });
}

void MainMenuScene::update() {
	client.game->textures.update();
	sandbox.update(client.getDelta());
	
	components->handleMouseInput(client.renderer.window);
}

void MainMenuScene::draw() {
	client.renderer.beginChunkDeferredCalls();
	client.renderer.endDeferredCalls();
	
	client.renderer.beginGUIDrawCalls();
	client.renderer.enableTexture(&client.game->textures.atlasTexture);
	components->draw(client.renderer);
}

void MainMenuScene::cleanup() {
	client.renderer.window.setCursorHand(false);
	client.renderer.window.removeResizeCallback("mainmenu");
}