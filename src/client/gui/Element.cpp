#include "Element.h"

#include "util/Util.h"
#include "client/gui/Root.h"
#include "client/graph/Renderer.h"

Gui::Element::~Element() {
	for (let& child : children) child->parent = nullptr;
}

const Gui::Props& Gui::Element::getProps() const {
	return props;
}

void Gui::Element::setProps(const Props& props) {
	this->props = props;
	updateElement();
}

void Gui::Element::setProp(Gui::Prop prop, const any& value) {
	props.props[prop] = value;
}

sptr<Gui::Element> Gui::Element::get(u32 ind) {
	if (ind >= children.size()) return nullptr;
	return children[ind];
}

sptr<Gui::Element> Gui::Element::get(const string& id) {
	for (let& child : children) {
		if (child->props.get<string>(Prop::ID) == id) return child;
	}
	for (let& child : children) {
		let res = child->get(id);
		if (res != nullptr) return res;
	}
	return nullptr;
}

void Gui::Element::clear() {
	children.clear();
}

void Gui::Element::remove() {
	if (parent) {
		for (let it = parent->children.begin(); it != parent->children.end(); it++) {
			if (it->get() == this) {
				parent->children.erase(it);
				break;
			}
		}
		parent = nullptr;
	}
}

void Gui::Element::onClick(const std::function<void(i32, bool)>& cb) {
	clickCb = cb;
}

Gui::ExpressionInfo Gui::Element::getExpr() const {
	return {
		parent ? parent->getComputedSize() : ivec2 {},
		getComputedSize()
	};
}

ivec2 Gui::Element::getComputedSize() const {
	let size = getStyleWithExpr<vec2, Type::LENGTH>(Prop::SIZE, vec2(nanf("")),
		{ parent ? parent->computedSize : ivec2 {}, {} });
	if (std::isnan(size.x)) size.x = std::max(layoutSize.x, 0);
	if (std::isnan(size.y)) size.y = std::max(layoutSize.y, 0);
	computedSize = ivec2(size);
	return size;
}

ivec2 Gui::Element::getComputedContentSize() const {
	let size = getComputedSize();
	let padding = getStyle<ivec4, Type::LENGTH>(Prop::PADDING, {});
	return glm::max(ivec2 { size.x - padding.x - padding.z, size.y - padding.y - padding.w }, 0);
}

ivec2 Gui::Element::getExplicitSize() const {
	return getStyle<ivec2, Type::LENGTH>(Prop::SIZE, ivec2(-1));
}

ivec2 Gui::Element::getComputedPos() const {
	return getStyle<ivec2, Type::LENGTH>(Prop::POS, layoutPosition);
}

ivec2 Gui::Element::getComputedScreenPos() const {
	return getComputedPos() + parentOffset;
}

bool Gui::Element::handleMouseHover(ivec2 mousePos, bool& pointer) {
	bool childIntersects = false;
	for (let& child : children) if (child->handleMouseHover(mousePos, pointer)) childIntersects = true;
	
	ivec2 size = getComputedSize();
	ivec2 pos = getComputedScreenPos();
	bool intersects = mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + size.y;
	
	let cursor = getStyle<string>(Prop::CURSOR);
	if (intersects && cursor) pointer = *cursor == "pointer";
	
	if (hovered != intersects) {
		hovered = intersects;
		updateElement();
	}
	
	return intersects || childIntersects;
}

bool Gui::Element::handleMouseClick(ivec2 mousePos, u32 button, bool down) {
	for (i32 i = children.size() - 1; i >= 0; i--)
		if (children[i]->handleMouseClick(mousePos, button, down)) return true;

	ivec2 size = getComputedSize();
	ivec2 pos = getComputedScreenPos();
	bool intersects = mousePos.x >= pos.x && mousePos.x <= pos.x + size.x &&
		mousePos.y >= pos.y && mousePos.y <= pos.y + size.y;
	if (!intersects) return false;

	if (clickCb) clickCb(button, down);
	return clickCb != nullptr;
}

void Gui::Element::draw(Renderer& renderer) {
	entity.draw(renderer);
	for (let& child : children) child->draw(renderer);
}

void Gui::Element::updateElement() {
	layoutChildren();
}

void Gui::Element::layoutChildren() {
	const string& layout = getStyle<string>(Prop::LAYOUT, "");
	
	switch (Util::hash(layout.data())) {
	default:
	case Util::hash("flex"): {
		const string& direction = getStyle<string>(Prop::DIRECTION, "");
	
		/**
		 * The primary flex direction. Stored as a bool but interpreted as an index into a vec2.
		 * 1 if the direction is column, 0 if it is row, indexes into `y` if column, `x` otherwise.
		 */
	 
		const bool primary = direction != "row";
	
		const string& hAlignRaw = getStyle<string>(Prop::H_ALIGN, "");
		const string& vAlignRaw = getStyle<string>(Prop::V_ALIGN, "");
	
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
	
		const i32 gap = getStyle<ivec2, Type::LENGTH>(Prop::GAP, ivec2(0))[primary];
		const ivec4& padding = getStyle<ivec4, Type::LENGTH>(Prop::PADDING, ivec4 {});
	
		/*
		 * Calculates the explicit spaced used up by children across the primary axis,
		 * i.e. the space that is defined using WIDTH or HEIGHT.
		 * Counts the number of elements without explicitely defined primary height.
		 */
	
		let selfSize = getComputedContentSize();
		i32 explicitSize = gap * (children.size() - 1);
		i32 implicitCount = 0;
	
		for (const let& child : children) {
			let childExplicitSize = child->getExplicitSize();
			if (childExplicitSize[primary] != -1) explicitSize += childExplicitSize[primary];
			else implicitCount++;
			let childMargin = child->getStyle<ivec4, Type::LENGTH>(Prop::MARGIN, {});
			explicitSize += childMargin[primary] + childMargin[primary + 2];
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
	 
		i32 implicitElemSize = floor((selfSize[primary] - explicitSize) / (std::max)(implicitCount, 1));
	
		ivec2 selfOffset = getComputedPos() + parentOffset;
		
		/**
		 * Position each child according to `offset`, size implicitly sized elements using `implicitElemSize`.
		 */
	
		for (const let& child : children) {
			let childExplicitSize = child->getExplicitSize();
			let childMargin = child->getStyle<ivec4, Type::LENGTH>(Prop::MARGIN, {});
		
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
				? implicitElemSize : childExplicitSize[primary])
				+ gap + childMargin[primary] + childMargin[primary + 2];
			
			child->parentOffset = selfOffset;
			
			child->updateElement();
		}
		break;
	}
	}
}