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
			{ Style::Rule::WIDTH, size.x },
			{ Style::Rule::HEIGHT, size.y }
		}}
	});
	
	lock = window.onResize([&](ivec2 size) {
		size = glm::ceil(vec2(window.getSize()) / static_cast<f32>(Gui::PX_SCALE));
		body->setStyle(Style::Rule::WIDTH, size.x);
		body->setStyle(Style::Rule::HEIGHT, size.y);
		body->setStyle(Style::Rule::BACKGROUND, vec4(rand(), 0, 0, 1));
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
	
}

void Gui::Root::draw(Renderer& renderer) {
	if (!body) return;
	body->draw(renderer);
}
