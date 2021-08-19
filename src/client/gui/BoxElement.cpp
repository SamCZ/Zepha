#include "BoxElement.h"

#include "client/gui/Root.h"
#include "client/graph/Model.h"
#include "game/atlas/asset/AtlasRef.h"
#include "client/graph/mesh/EntityMesh.h"

void Gui::BoxElement::updateElement() {
	const ivec2 pos = getComputedPos() * static_cast<i32>(PX_SCALE);
	const ivec2 size = getComputedSize() * static_cast<i32>(PX_SCALE);
	const string& backgroundImage = getStyle<string>(Style::Rule::BACKGROUND_IMAGE, "");
	const vec4 backgroundColor = getStyle<vec4, Style::Type::COLOR>(Style::Rule::BACKGROUND_COLOR, {{}});
	
	auto mesh = std::make_unique<EntityMesh>();
	if (!backgroundImage.empty()) {
		const let& tex = *root.atlas[backgroundImage];
		mesh->create({
			{ { 0, 0, 0 }, { tex.uv.x, tex.uv.y, 0, 1 }, vec3(1), true, {}, {}, {} },
			{ { 0, size.y, 0 }, { tex.uv.x, tex.uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
			{ { size.x, size.y, 0 }, { tex.uv.z, tex.uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
			{ { size.x, 0, 0 }, { tex.uv.z, tex.uv.y, 0, 1 }, vec3(1), true, {}, {}, {} }
		}, { 0, 1, 2, 2, 3, 0 });
	}
	else if (backgroundColor.a != 0) {
		mesh->create({
			{ { 0, 0, 0 }, backgroundColor, vec3(1), false, {}, {}, {} },
			{ { 0, size.y, 0 }, backgroundColor, vec3(1), false, {}, {}, {} },
			{ { size.x, size.y, 0 }, backgroundColor, vec3(1), false, {}, {}, {} },
			{ { size.x, 0, 0 }, backgroundColor, vec3(1), false, {}, {}, {} }
		}, { 0, 1, 2, 2, 3, 0 });
	}
	
	auto model = make_shared<Model>();
	model->fromMesh(std::move(mesh));
	entity.setModel(model);
	entity.setPos(vec3(pos, 0) + (parent ? parent->entity.getPos() : vec3 {}));
	
	layoutChildren();
}

void Gui::BoxElement::layoutChildren() {
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
		else if (align[primary] == 0) offset[primary] += selfSize[primary] / 2 - explicitSize / 2 - (gap * (children.size() - 1)) / 2;
		
		/**
		 * The amount of size each implicitly sized element should occupy.
		 */
		 
		i32 implicitElemSize = floor((selfSize[primary] - explicitSize) /
			(std::max)(implicitCount, static_cast<usize>(1)));
		
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

			child->updateElement();
		}
		break;
	}
	}
}