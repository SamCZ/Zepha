//#include "GuiCellGraph.h"
//
//#include "client/gui/basic/GuiRect.h"
//#include "client/gui/basic/GuiText.h"
//#include "client/gui/basic/GuiCells.h"
//
//void GuiCellGraph::create(f32 scale, vec4 padding, u16 count, const string& title, Font font) {
//	this->scale = vec2(scale * count + padding.x + padding.z, scale * count + padding.y + padding.w + 28);
//	this->padding = padding;
//	this->title = title;
//	this->count = count;
//	this->font = std::move(font);
//
//	let background = make_shared<GuiRect>();
//	background->create(this->scale, {}, { 0.1, 0.1, 0.1, 0.2 }, { 0.1, 0.1, 0.1, 0.2 },
//		{ 0.1, 0.1, 0.1, 0.7 },{ 0.1, 0.1, 0.1, 0.7 });
//	background->setPos({ 0, 0 });
//	add(background);
//
//	let label = make_shared<GuiText>();
//	label->create({ 2, 2 }, {}, { 1, 1, 1, 1 }, {}, this->font);
//	label->setPos({ 4, 8 });
//	label->setText(title);
//	add(label);
//
//	cells = std::make_shared<GuiCells>();
//	cells->create(scale, count);
//	cells->setPos(vec2(padding.x, padding.y + 28));
//	add(cells);
//}
//
//void GuiCellGraph::setCellColor(u16vec2 pos, vec4 color) {
//	cells->setCellColor(pos, color);
//}
//
//void GuiCellGraph::refresh() {
//	cells->refresh();
//}
