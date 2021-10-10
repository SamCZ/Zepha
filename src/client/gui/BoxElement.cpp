#include "BoxElement.h"

#include "client/gui/Root.h"
#include "client/graph/Model.h"
#include "client/graph/mesh/EntityMesh.h"
#include "game/atlas/asset/AtlasTexture.h"

void Gui::BoxElement::updateElement() {
	const let bgRule = hovered && getStyle(Prop::BACKGROUND_HOVER) ?
		Prop::BACKGROUND_HOVER : Prop::BACKGROUND;
	
	let rawBg = getStyle(bgRule);
	
	bool isDirty = false;
	if ((!rawBg && curBg) || (rawBg && !curBg)) isDirty = true;
	else if (rawBg && curBg) {
		if (rawBg->type() != curBg->type()) isDirty = true;
		else if (rawBg->type() == typeid(vec4)) isDirty = any_cast<vec4>(*rawBg) != any_cast<vec4>(*curBg);
		else if (rawBg->type() == typeid(string)) isDirty = any_cast<string>(*rawBg) != any_cast<string>(*curBg);
	}
	curBg = rawBg;
	
	if (isDirty) {
		const let bgColor = getStyle<vec4, Type::COLOR>(bgRule);
		const string bgImage = getStyle<string>(bgRule, "");
		
		if ((!bgColor || bgColor->a == 0) && bgImage.empty()) entity.setModel(nullptr);
		else if (bgColor) {
			let mesh = make_unique<EntityMesh>();
			
			tex = {};
			mesh->create({
				{ { 0, 0, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 0, 1, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 1, 1, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 1, 0, 0 }, *bgColor, vec3(1), false, {}, {}, {} }
			}, { 0, 1, 2, 2, 3, 0 });
			
			let model = make_shared<Model>();
			model->fromMesh(std::move(mesh));
			entity.setModel(model);
		}
		else {
			let mesh = make_unique<EntityMesh>();
			
			tex = root.atlas[bgImage];
			let uv = tex->getUVPos();
			mesh->create({
				{ { 0, 0, 0 }, { uv.x, uv.y, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 0, 1, 0 }, { uv.x, uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 1, 1, 0 }, { uv.z, uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 1, 0, 0 }, { uv.z, uv.y, 0, 1 }, vec3(1), true, {}, {}, {} }
			}, { 0, 1, 2, 2, 3, 0 });
			
			let model = make_shared<Model>();
			model->fromMesh(std::move(mesh));
			entity.setModel(model);
		}
	}
	
	let margin = getStyle<vec4, Type::LENGTH>(Prop::MARGIN, {});
	
	entity.setScale(ivec3(getComputedSize(), 0));
	entity.setPos(ivec3(getComputedScreenPos() + vec2 { margin.x, margin.y }, 0));
	
	Element::updateElement();
}