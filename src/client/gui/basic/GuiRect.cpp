////
//// Created by aurailus on 10/02/19.
////
//
//#include "GuiRect.h"
//
//#include "util/Util.h"
//#include "client/graph/Model.h"
//#include "client/gui/SerialGui.h"
//#include "game/atlas/TextureAtlas.h"
//#include "game/atlas/asset/AtlasRef.h"
//
//std::shared_ptr<GuiRect> GuiRect::fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, glm::ivec2 bounds) {
//	glm::vec2 pos = SerialGui::get<glm::vec2>(elem, "position", bounds);
//	glm::vec2 offset = SerialGui::get<glm::vec2>(elem, "position_anchor");
//	glm::vec2 size = SerialGui::get<glm::vec2>(elem, "size", bounds);
//	glm::vec4 padding = SerialGui::get<glm::vec4>(elem, "padding", bounds);
//
//	pos -= offset * size;
//	size -= glm::vec2{ padding.y + padding.w, padding.x + padding.z };
//
//	std::string background = elem.get_or<std::string>("background", "");
//	bool hideOverflow = elem.get_or<std::string>("overflow", "visible") == "hidden";
//
//	auto rect = std::make_shared<GuiRect>(window, elem.key);
//	if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
//	else if (background.size() > 0) rect->create(size, padding, textures[background]);
//	else rect->create(size, padding, glm::vec4{});
//	rect->setOverflows(!hideOverflow);
//	rect->setPos(pos);
//	return rect;
//}
//
//// Single Color Constructor
//// Creates a GuiRect object whose background
//// is a flat color defined by 'color'.
//void GuiRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color) {
//	this->scale = scale;
//	this->padding = padding;
//
//	auto mesh = std::make_unique<EntityMesh>();
//	mesh->create({{{ 0, 0, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 0, 1, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 1, 1, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 1, 0, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}}
//	}, { 0, 1, 2, 2, 3, 0 });
//	auto model = std::make_shared<Model>();
//	model->fromMesh(std::move(mesh));
//
//	entity.setModel(model);
//	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
//}
//
//// Multiple Color Constructor
//// Creates a GuiRect object with a gradient background
//// defined by 'tl', 'tr', 'bl', and 'br'.
//void GuiRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
//	this->scale = scale;
//	this->padding = padding;
//
//	auto mesh = std::make_unique<EntityMesh>();
//	mesh->create({
//		{{ 0, 0, 0 }, tl, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 0, 1, 0 }, bl, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 1, 1, 0 }, br, { 1, 1, 1 }, false, {}, {}, {}},
//		{{ 1, 0, 0 }, tr, { 1, 1, 1 }, false, {}, {}, {}}
//	}, { 0, 1, 2, 2, 3, 0 });
//	auto model = std::make_shared<Model>();
//	model->fromMesh(std::move(mesh));
//
//	entity.setModel(model);
//	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
//}
//
//// Texture Constructor
//// Creates a GuiRect object with a textured background
//// defined by the 'texture' reference.
//void GuiRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture) {
//	this->scale = scale;
//	this->padding = padding;
//	this->texture = texture;
//	this->hitbox = scale + glm::vec2{ padding.y + padding.w, padding.x + padding.z };
//
//	auto mesh = std::make_unique<EntityMesh>();
//	mesh->create({
//		{{ 0, 0, 0 }, { this->texture->uv.x, this->texture->uv.y, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 0, 1, 0 }, { this->texture->uv.x, this->texture->uv.w, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 1, 1, 0 }, { this->texture->uv.z, this->texture->uv.w, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 1, 0, 0 }, { this->texture->uv.z, this->texture->uv.y, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}}
//	}, { 0, 1, 2, 2, 3, 0 });
//	auto model = std::make_shared<Model>();
//	model->fromMesh(std::move(mesh));
//
//	entity.setModel(model);
//	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
//}
//
//// Texture Constructor
//// Creates a GuiRect object with a textured background
//// defined by the 'texture' reference.
//void GuiRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, glm::vec4 tint) {
//	this->scale = scale;
//	this->padding = padding;
//	this->texture = std::move(texture);
//
//	auto mesh = std::make_unique<EntityMesh>();
//	mesh->create({
//		{{ 0, 0, 0 }, { this->texture->uv.x, this->texture->uv.y, 0, tint.w }, glm::vec3{ tint }, true, {}, {}, {}},
//		{{ 0, 1, 0 }, { this->texture->uv.x, this->texture->uv.w, 0, tint.w }, glm::vec3{ tint }, true, {}, {}, {}},
//		{{ 1, 1, 0 }, { this->texture->uv.z, this->texture->uv.w, 0, tint.w }, glm::vec3{ tint }, true, {}, {}, {}},
//		{{ 1, 0, 0 }, { this->texture->uv.z, this->texture->uv.y, 0, tint.w }, glm::vec3{ tint }, true, {}, {}, {}}
//	}, { 0, 1, 2, 2, 3, 0 });
//	auto model = std::make_shared<Model>();
//	model->fromMesh(std::move(mesh));
//
//	entity.setModel(model);
//	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
//}
