//
// Created by aurailus on 2019-12-28.
//

#include "LuaErrorScene.h"

#include "client/Client.h"
#include "client/graph/Font.h"
#include "client/graph/Renderer.h"
#include "client/gui/basic/GuiRect.h"
#include "client/gui/basic/GuiText.h"

LuaErrorScene::LuaErrorScene(Client& client, const std::string& err) : Scene(client), err(err) {
	client.renderer.setClearColor(0, 0, 0);
	client.renderer.window.input.lockMouse(false);
	
	Font f(client.game->textures, client.game->textures["font"]);
	glm::ivec2 win = client.renderer.window.getSize();
	
	auto container = std::make_shared<GuiRect>("container");
	container->create({ 800, 500 }, {}, { 0.05, 0.05, 0.05, 1 });
	container->setPos({ win.x / 2 - 800 / 2, win.y / 2 - 500 / 2 });
	components.add(container);
	
	auto titleText = std::make_shared<GuiText>("titleText");
	titleText->create({ 3, 3 }, {}, {}, { 1, 0.4, 0.5, 1 }, f);
	titleText->setText("The Zepha sandbox has encountered an error.");
	titleText->setPos({ 16, 12 });
	container->add(titleText);
	
	auto errorText = std::make_shared<GuiText>("errorText");
	errorText->create({ 2, 2 }, {}, {}, { 0.85, 0.85, 0.85, 1 }, f);
	errorText->setText(err);
	errorText->setPos({ 16, 48 });
	container->add(errorText);
	
	client.renderer.window.addResizeCallback("scene", [&](glm::ivec2 win) {
		components.get<GuiRect>("container")->setPos({ win.x / 2 - 800 / 2, win.y / 2 - 500 / 2 });
	});
}

void LuaErrorScene::update() {
	client.game->textures.update();
}

void LuaErrorScene::draw() {
	Renderer& renderer = client.renderer;
	
	renderer.beginChunkDeferredCalls();
	renderer.endDeferredCalls();
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	
	components.draw(renderer);
}

void LuaErrorScene::cleanup() {
	client.renderer.window.removeResizeCallback("scene");
}