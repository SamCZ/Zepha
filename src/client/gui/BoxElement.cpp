#include "BoxElement.h"

#include "client/gui/Root.h"
#include "client/graph/Model.h"
#include "game/atlas/asset/AtlasRef.h"
#include "client/graph/mesh/EntityMesh.h"

void Gui::BoxElement::updateElement() {
	const let bgRule = hovered && getStyle(StyleRule::BACKGROUND_HOVER) ?
		StyleRule::BACKGROUND_HOVER : StyleRule::BACKGROUND;
	
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
		const let bgColor = getStyle<vec4, ValueType::COLOR>(bgRule);
		const string bgImage = getStyle<string>(bgRule, "");
		
		let mesh = std::make_unique<EntityMesh>();
		if (bgColor && bgColor->a != 0) {
			tex = nullptr;
			mesh->create({
				{ { 0, 0, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 0, 1, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 1, 1, 0 }, *bgColor, vec3(1), false, {}, {}, {} },
				{ { 1, 0, 0 }, *bgColor, vec3(1), false, {}, {}, {} }
			}, { 0, 1, 2, 2, 3, 0 });
		}
		else if (!bgImage.empty()) {
			tex = root.atlas[bgImage];
			mesh->create({
				{ { 0, 0, 0 }, { tex->uv.x, tex->uv.y, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 0, 1, 0 }, { tex->uv.x, tex->uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 1, 1, 0 }, { tex->uv.z, tex->uv.w, 0, 1 }, vec3(1), true, {}, {}, {} },
				{ { 1, 0, 0 }, { tex->uv.z, tex->uv.y, 0, 1 }, vec3(1), true, {}, {}, {} }
			}, { 0, 1, 2, 2, 3, 0 });
		}
	
		let model = make_shared<Model>();
		model->fromMesh(std::move(mesh));
		entity.setModel(model);
	}
	
	entity.setScale(vec3(getComputedSize() * static_cast<i32>(PX_SCALE), 0));
	entity.setPos(vec3(getComputedScreenPos() * static_cast<i32>(PX_SCALE), 0));
	
	Element::updateElement();
}