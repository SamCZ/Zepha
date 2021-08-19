#include "Element.h"

#include "util/Util.h"
#include "client/graph/Renderer.h"

Gui::Element::~Element() {
	for (let& child : children) child->parent = nullptr;
}

void Gui::Element::setProps(const Props& props) {
	this->props = props;
	updateElement();
}

const std::any& Gui::Element::getStyle(Style::Rule style) const {
	return props.styles.get(style);
}

void Gui::Element::setStyle(Style::Rule style, const std::any& value) {
	props.styles.rules[style] = value;
	updateElement();
}

ivec2 Gui::Element::getComputedSize() {
	return {
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::WIDTH, std::max(layoutSize.x, 0)),
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::HEIGHT, std::max(layoutSize.y, 0))
	};
}

ivec2 Gui::Element::getComputedContentSize() {
	let size = getComputedSize();
	let padding = props.styles.get<ivec4>(Style::Rule::PADDING, ivec4 {});
	return glm::max(ivec2 { size.x - padding.x - padding.z, size.y - padding.y - padding.w }, ivec2 {});
}

ivec2 Gui::Element::getExplicitSize() {
	return {
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::WIDTH, -1),
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::HEIGHT, -1)
	};
}

ivec2 Gui::Element::getComputedPos() {
	return {
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::LEFT, layoutPosition.x),
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::TOP, layoutPosition.y)
	};
}

void Gui::Element::handleMouseMove(ivec2 mousePos) {
	for (let& child : children) child->handleMouseMove(mousePos);
}

bool Gui::Element::handleMouseClick(u32 button, bool down) {
	for (let& child: children) if (child->handleMouseClick(button, down)) return true;
	return false;
}

void Gui::Element::draw(Renderer& renderer) {
	entity.draw(renderer);
	for (let& child : children) child->draw(renderer);
}