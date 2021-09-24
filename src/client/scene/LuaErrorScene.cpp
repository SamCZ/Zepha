#include "LuaErrorScene.h"

#include "util/Types.h"
#include "client/Client.h"
#include "client/graph/Renderer.h"
#include "client/gui/TextElement.h"
#include "client/scene/MainMenuScene.h"

LuaErrorScene::LuaErrorScene(Client& client, const std::string& err) : Scene(client),
	root(client.renderer.window, client.game->textures) {
	
	client.renderer.setClearColor(0, 0, 0);
	client.renderer.window.input.setMouseLocked(false);
	
	using Expr = Gui::Expression;
	root.body->append<Gui::TextElement>({{
		{ Gui::Prop::TEXT_SIZE, Expr("2px") },
		{ Gui::Prop::SIZE, array<Expr, 2> { Expr("100dp"), Expr("-1") } },
		{ Gui::Prop::CONTENT, string("`cfEncountered a fatal mod error ;-;\n\n`r") + err },
		{ Gui::Prop::MARGIN, array<Expr, 4> { Expr("4dp"), Expr("4dp"), Expr("4dp"), Expr("4dp") } }
	}});
	
	root.body->onClick([&](i32 button, bool down) {
		if (button != GLFW_MOUSE_BUTTON_1 || !down) return;
		client.scene.setScene(make_unique<MainMenuScene>(client));
	});
}

void LuaErrorScene::update() {
	root.update();
}

void LuaErrorScene::draw() {
	Renderer& renderer = client.renderer;
	
	renderer.beginChunkDeferredCalls();
	renderer.endDeferredCalls();
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.texture);
	
	root.draw(renderer);
}