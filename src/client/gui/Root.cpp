#include "Root.h"

#include "util/Types.h"
#include "util/Timer.h"
#include "client/gui/BoxElement.h"

Gui::Root::Root(Window& window, TextureAtlas& atlas) :
	atlas(atlas),
	window(window),
	body(make_shared<BoxElement>(*this, stylesheets)) {
	const ivec2 size = window.getSize();
	
	using Expr = Expression;
	body->setProps({{
		{ Prop::ID, string("body") },
		{ Prop::SIZE, array<Expr, 2> { Expr(std::to_string(size.x)), Expr(std::to_string(size.y)) }}
	}});
	
	callbacks.emplace_back(window.resize.bind([&](ivec2 size) {
		body->setProp(Prop::SIZE, array<Expr, 2> {
			Expr(std::to_string(size.x)), Expr(std::to_string(size.y)) });
		Timer t("Resize UI");
		body->updateElement();
		t.printElapsedMs();
	}));
	
	callbacks.emplace_back(window.input.events.bind(Input::CBType::MOUSE, [&](u32 button, i32 state) {
		let pos = window.input.getMousePos();
		body->handleMouseClick(pos, button, state == GLFW_PRESS);
	}));
}

Gui::Root::~Root() {
	window.setCursorHand(false);
}

void Gui::Root::addStylesheet(const StyleSheet& sheet) {
	stylesheets.emplace_back(sheet);
}

void Gui::Root::update() {
	bool pointer = false;
	const let pos = window.input.getMousePos();
	body->handleMouseHover(pos, pointer);
	window.setCursorHand(pointer);
}

void Gui::Root::draw(Renderer& renderer) {
	if (!body) return;
	body->draw(renderer);
}