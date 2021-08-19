////
//// Created by aurailus on 2019-12-11.
////
//
//#include "GuiImageButton.h"
//
//#include "game/LocalSubgame.h"
//#include "client/gui/SerialGui.h"
//#include "client/gui/basic/GuiText.h"
//#include "game/atlas/asset/AtlasRef.h"
//
//std::shared_ptr<GuiImageButton>
//GuiImageButton::fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, glm::ivec2 bounds) {
//	glm::vec2 pos = SerialGui::get<glm::vec2>(elem, "position", bounds);
//	glm::vec2 offset = SerialGui::get<glm::vec2>(elem, "position_anchor");
//	glm::vec2 size = SerialGui::get<glm::vec2>(elem, "size", bounds);
//	glm::vec4 padding = SerialGui::get<glm::vec4>(elem, "padding", bounds);
//
//	pos -= offset * size;
//	size -= glm::vec2{ padding.y + padding.w, padding.x + padding.z };
//
//	std::string background = elem.get_or<std::string>("background", "");
//	std::string background_hover = elem.get_or<std::string>("background_hover", background);
//
//	bool hideOverflow = elem.get_or<std::string>("overflow", "visible") == "hidden";
//	std::string content = elem.get_or<std::string>("content", "");
//
//	auto button = std::make_shared<GuiImageButton>(window, elem.key);
//	button->create(size, padding, textures[background], textures[background_hover]);
//	button->setOverflows(!hideOverflow);
//	button->setPos(pos);
//
//	if (content != "") {
//		auto text = std::make_shared<GuiText>(window, elem.key + "__TEXT");
//		text->create(glm::vec2(SerialGui::SCALE_MODIFIER), padding, {}, { 1, 1, 1, 1 }, { textures, textures["font"] });
//		text->setPos({ 6 * SerialGui::SCALE_MODIFIER, size.y / 2 - 4.5 * SerialGui::SCALE_MODIFIER });
//		text->setText(content);
//		button->add(text);
//	}
//
//	return button;
//}
//
//// Texture Constructor
//// Creates a GuiImageButton object with two textures
//// defined by the 'texture' & 'hoverTexture' reference.
//void GuiImageButton::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture,
//	std::shared_ptr<AtlasRef> hoverTexture) {
//	this->hoverTexture = hoverTexture;
//	GuiRect::create(scale, padding, texture);
//
//	setCallback(Element::CallbackType::HOVER, nullptr);
//}
//
//void GuiImageButton::setCallback(Element::CallbackType type, const Element::Callback& cb) {
//	if (type == CallbackType::HOVER) {
//		Element::setCallback(type, [&, cb](bool nowHovered, glm::ivec2 pos) {
//			if (cb) cb(nowHovered, pos);
//			if (nowHovered != hovered) this->rebuild(nowHovered);
//		});
//	}
//	else Element::setCallback(type, cb);
//}
//
//void GuiImageButton::rebuild(bool hover) {
//	auto tex = (hover) ? (hoverTexture != nullptr) ? hoverTexture : texture : texture;
//
//	auto mesh = std::make_unique<EntityMesh>();
//	mesh->create({
//		{{ 0, 0, 0 }, { tex->uv.x, tex->uv.y, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 0, 1, 0 }, { tex->uv.x, tex->uv.w, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 1, 1, 0 }, { tex->uv.z, tex->uv.w, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}},
//		{{ 1, 0, 0 }, { tex->uv.z, tex->uv.y, 0, 1 }, { 1, 1, 1 }, true, {}, {}, {}}
//	}, { 0, 1, 2, 2, 3, 0 });
//
//	auto model = std::make_shared<Model>();
//	model->fromMesh(std::move(mesh));
//
//	entity.setModel(model);
//	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
//}