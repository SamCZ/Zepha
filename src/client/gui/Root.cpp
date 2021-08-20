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
			{ StyleRule::SIZE, array<string, 2> {
				std::to_string(size.x) + "px", std::to_string(size.y) + "px" } }
		}}
	});
	
	lock = window.onResize([&](ivec2 size) {
		body->setStyle(StyleRule::SIZE, array<string, 2> {
			std::to_string(size.x) + "px", std::to_string(size.y) + "px" });
		Timer t("Resize UI");
		body->updateElement();
		t.printElapsedMs();
	});
	
//	window.input.bindMouseCallback()
}

Gui::Root::~Root() {
	window.setCursorHand(false);
}

void Gui::Root::addStylesheet(const std::unordered_map<string, Style>& sheet) {
	stylesheets.emplace_back(sheet);
}

void Gui::Root::update() {
	const let pos = window.input.getMousePos();
	bool pointer = false;
	body->handleMouseHover(pos, pointer);
	setCursorPointer(pointer);
}

void Gui::Root::draw(Renderer& renderer) {
	if (!body) return;
	body->draw(renderer);
}

void Gui::Root::setCursorPointer(bool hand) {
	window.setCursorHand(hand);
}