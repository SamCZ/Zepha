#include "GuiPerfGraph.h"

#include "client/gui/basic/GuiRect.h"
#include "client/gui/basic/GuiText.h"
#include "client/gui/basic/GuiMeter.h"
#include "world/dim/ent/DrawableEntity.h"

void GuiPerfGraph::create(f32 scale, vec4 padding, const vec<string>& sections, const string& title, Font font) {
	const static int GRAPH_PAD_X = 2;
	const static int GRAPH_PAD_Y = 62;
	const static int TEXT_PAD_X = 4;
	const static int TEXT_PAD_Y = 8;
	
	this->scale = vec2(scale, 64 + ceil((sections.size() + 1) / 2) * 24 + 6);
	this->padding = padding;
	this->title = title;
	this->sections = sections;
	this->font = std::move(font);
	
	auto background = make_shared<GuiRect>("background");
	background->create(this->scale, {}, { 0.1, 0.1, 0.1, 0.2 }, { 0.1, 0.1, 0.1, 0.2 },
		{ 0.1, 0.1, 0.1, 0.7 },{ 0.1, 0.1, 0.1, 0.7 });
	add(background);
	background->setPos({ 0, 0 });
	
	auto meter = make_shared<GuiMeter>("meter");
	meter->create({ scale - GRAPH_PAD_X * 2, 24 }, {}, 0.85);
	add(meter);
	meter->setBudget(1 / 60.f * 1000000000);
	meter->setPos({ GRAPH_PAD_X, GRAPH_PAD_Y });
	
	auto label = std::make_shared<GuiText>("label");
	label->create({ 2, 2 }, {}, { 1, 1, 1, 1 }, {}, this->font);
	label->setText(title);
	add(label);
	label->setPos({ TEXT_PAD_X, TEXT_PAD_Y });
	
	for (usize i = 0; i < sections.size(); i++) {
		auto label = std::make_shared<GuiText>();
		label->create({ 2, 2 }, {}, vec4(GuiMeter::COLORS[i % GuiMeter::COLORS.size()], 1), {}, this->font);
		label->setText(sections[i]);
		add(label);
		label->setPos({ TEXT_PAD_X + scale * (i % 2) / 2, TEXT_PAD_Y + GRAPH_PAD_Y + (i / 2) * 24 + 2 });
	}
}

void GuiPerfGraph::updateTimings(const vec<usize>& timings) {
	get<GuiMeter>("meter")->updateValues(timings);
}
