#include "Root.h"

#include "util/Types.h"
#include "util/Timer.h"
#include "client/gui/BoxElement.h"

Gui::Root::Root(Window& window, TextureAtlas& atlas) :
	atlas(atlas),
	window(window),
	body(make_shared<BoxElement>(*this, stylesheets)) {
	const ivec2 size = window.getSize();
	
	body->setProps({
		.id = "body",
		.styles = {{
			{ StyleRule::SIZE, array<Expression, 2> {
				Expression(std::to_string(size.x)), Expression(std::to_string(size.y)) }}
		}}
	});
	
	callbacks.emplace_back(window.resize.bind([&](ivec2 size) {
		body->setStyle(StyleRule::SIZE, array<Expression, 2> {
			Expression(std::to_string(size.x)), Expression(std::to_string(size.y)) });
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

void Gui::Root::addStylesheet(const std::unordered_map<string, Style>& sheet) {
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