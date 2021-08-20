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

void Gui::Element::setStyle(Style::Rule style, const std::any& value) {
	props.styles.rules[style] = value;
}

ivec2 Gui::Element::getComputedSize() {
	return {
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::WIDTH, std::max(layoutSize.x, 0)),
		getStyle<i32, Style::Type::LENGTH>(Style::Rule::HEIGHT, std::max(layoutSize.y, 0))
	};
}

ivec2 Gui::Element::getComputedContentSize() {
	let size = getComputedSize();
	let padding = getStyle<ivec4>(Style::Rule::PADDING, {});
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

void Gui::Element::updateElement() {
	layoutChildren();
}

void Gui::Element::layoutChildren() {
	const string& layout = getStyle<string>(Style::Rule::LAYOUT, "");
	
	switch (Util::hash(layout.data())) {
	default:
	case Util::hash("flex"): {
		const string& direction = getStyle<string>(Style::Rule::DIRECTION, "");
	
		/**
		 * The primary flex direction. Stored as a bool but interpreted as an index into a vec2.
		 * 1 if the direction is column, 0 if it is row, indexes into `y` if column, `x` otherwise.
		 */
	 
		const bool primary = direction != "row";
	
		const string& hAlignRaw = getStyle<string>(Style::Rule::H_ALIGN, "");
		const string& vAlignRaw = getStyle<string>(Style::Rule::V_ALIGN, "");
	
		/**
		 * Parsed alignment of the horizontal and vertical axes.
		 * -1: Start, 0: Center, 1: End, 2: Stretch
		 */
	 
		const i8vec2 align = {
			hAlignRaw == "left" ? -1 : hAlignRaw == "right" ? 1 : hAlignRaw == "center" ? 0 : 2,
			vAlignRaw == "top" ? -1 : vAlignRaw == "bottom" ? 1 : vAlignRaw == "center" ? 0 : 2
		};
	
		/**
		 * The element gap across the primary axis.
		 */
	
		const i32 gap = getStyle<ivec2>(Style::Rule::GAP, ivec2(0))[primary];
		const ivec4& padding = getStyle<ivec4>(Style::Rule::PADDING, ivec4 {});
	
		/*
		 * Calculates the explicit spaced used up by children across the primary axis,
		 * i.e. the space that is defined using WIDTH or HEIGHT.
		 * Counts the number of elements without explicitely defined primary height.
		 */
	
		let selfSize = getComputedContentSize();
		i32 explicitSize = gap * (children.size() - 1);
		usize implicitCount = 0;
	
		for (const let& child : children) {
			let childExplicitSize = child->getExplicitSize();
			if (childExplicitSize[primary] != -1) explicitSize += childExplicitSize[primary];
			else implicitCount++;
		}
	
		/**
		 * The cumulative layout offset of the children across the x and y axis.
		 */
	
		ivec2 offset = { padding.x, padding.y };
		if (align[primary] == 1) offset[primary] += selfSize[primary] - explicitSize - (gap * (children.size() - 1));
		else if (align[primary] == 0) offset[primary] += selfSize[primary] / 2 -
			explicitSize / 2 - (gap * (children.size() - 1)) / 2;
	
		/**
		 * The amount of size each implicitly sized element should occupy.
		 */
	 
		i32 implicitElemSize = floor((selfSize[primary] - explicitSize) /
			(std::max)(implicitCount, static_cast<usize>(1)));
	
		ivec2 selfOffset = getComputedPos() + parentOffset;
		
		/**
		 * Position each child according to `offset`, size implicitly sized elements using `implicitElemSize`.
		 */
	
		for (const let& child : children) {
			let childExplicitSize = child->getExplicitSize();
		
			child->layoutSize[primary] =
				(childExplicitSize[primary] == -1 && align[primary] == 2) ? implicitElemSize : 0;
		
			if (align[!primary] == 2) child->layoutSize[!primary] = selfSize[!primary];
			else child->layoutSize[!primary] = -1;
		
			if (align[!primary] == 2 || align[!primary] == -1) child->layoutPosition[!primary] = offset[!primary];
			else if (align[!primary] == 0) child->layoutPosition[!primary] =
				selfSize[!primary] / 2 - childExplicitSize[!primary] / 2;
			else if (align[!primary] == 1) child->layoutPosition[!primary] =
				selfSize[!primary] - childExplicitSize[!primary];
			child->layoutPosition[primary] = offset[primary];
		
			offset[primary] += ((childExplicitSize[primary] == -1 && align[primary] == 2)
				? implicitElemSize : childExplicitSize[primary]) + gap;
			
			child->parentOffset = selfOffset;

			child->updateElement();
		}
		break;
	}
	}
}