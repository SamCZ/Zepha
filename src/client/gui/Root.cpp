#include "Root.h"

#include "util/Timer.h"
#include "client/gui/BoxElement.h"

Gui::Root::Root(Window& window, TextureAtlas& atlas) :
	atlas(atlas),
	window(window),
	body(make_shared<BoxElement>(*this, stylesheets)) {
	const ivec2 size = glm::ceil(vec2(window.getSize()) / static_cast<f32>(Gui::PX_SCALE));
	
	body->setProps({
		.id = "body",
		.styles = {{
			{ StyleRule::WIDTH, size.x },
			{ StyleRule::HEIGHT, size.y }
		}}
	});
	
	lock = window.onResize([&](ivec2 size) {
		size = glm::ceil(vec2(window.getSize()) / static_cast<f32>(Gui::PX_SCALE));
		body->setStyle(StyleRule::WIDTH, size.x);
		body->setStyle(StyleRule::HEIGHT, size.y);
		Timer t("Resize UI");
		body->updateElement();
		t.printElapsedMs();
	});
	
//	window.input.bindMouseCallback()
}

void Gui::Root::addStylesheet(const std::unordered_map<string, Style>& sheet) {
	stylesheets.emplace_back(sheet);
}

void Gui::Root::update() {
	const let pos = window.input.getMousePos();
	body->handleMouseHover(pos);
}

void Gui::Root::draw(Renderer& renderer) {
	if (!body) return;
	body->draw(renderer);
}
