//
// Created by aurailus on 2019-12-12.
//

#include "GuiBuilder.h"

#include "util/Util.h"
#include "client/gui/basic/GuiRect.h"
#include "client/gui/basic/GuiText.h"
#include "client/gui/basic/GuiModel.h"
#include "client/gui/basic/GuiContainer.h"
#include "client/gui/compound/GuiImageButton.h"

GuiBuilder::GuiBuilder(TextureAtlas& textures, ModelStore& models, std::shared_ptr<GuiContainer> root) :
	textures(textures), models(models), root(root) {}

void GuiBuilder::setGuiRoot(std::shared_ptr<LuaGuiElement> menu) {
	elements = menu;
}

void GuiBuilder::update() {
	if (dirty) {
		build();
		dirty = false;
	}
}

void GuiBuilder::build(glm::ivec2 winBounds) {
	clear(false);
	if (winBounds != glm::ivec2{}) this->winBounds = winBounds;
	if (elements) create(*elements, root, this->winBounds);
}

void GuiBuilder::clear(bool deleteRoot) {
	clearCallbacks(root);
	root->empty();
	if (deleteRoot) elements = nullptr;
}

void GuiBuilder::create(LuaGuiElement& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds) {
	if (element.get_or<bool>("visible", true)) {
		auto component = createComponent(element, bounds);
		if (!component) throw std::runtime_error("GuiBuilder failed to create component: " + element.key);
		parent->add(component);
		for (auto& child : element.children) create(*child, component, component->getScale());
	}
}

std::shared_ptr<GuiComponent> GuiBuilder::createComponent(LuaGuiElement& elem, glm::ivec2 bounds) {
	std::shared_ptr<GuiComponent> c = nullptr;
	
	switch (Util::hash(elem.type.c_str())) {
	default: break;
	case Util::hash("Body"): {
		auto body = GuiRect::fromSerialized(elem, textures, bounds);
		body->setScale(bounds);
		c = body;
		break;
	}
	case Util::hash("Rect"):c = GuiRect::fromSerialized(elem, textures, bounds);
		break;
	case Util::hash("Button"):c = GuiImageButton::fromSerialized(elem, textures, bounds);
		break;
	case Util::hash("Text"):c = GuiText::fromSerialized(elem, textures, bounds);
		break;
	case Util::hash("Model"):c = GuiModel::fromSerialized(elem, textures, models, bounds);
		break;
	}
	
	if (!c) return nullptr;
	
	elem.updateFunction = std::bind(&GuiBuilder::elementUpdated, this);

//    if (elem.callbacks.count("primary")) c->setCallback(GuiComponent::CallbackType::PRIMARY, [=](bool b, glm::vec2 v) {
//        elem.callbacks.at("primary")(b, LuaParser::luaVec(elem.callbacks.at("primary").lua_state(), {v.x, v.y, 0})); });
//
//    if (elem.callbacks.count("secondary")) c->setCallback(GuiComponent::CallbackType::SECONDARY, [=](bool b, glm::vec2 v) {
//        elem.callbacks.at("secondary")(b, LuaParser::luaVec(elem.callbacks.at("secondary").lua_state(), {v.x, v.y, 0})); });
//
//    if (elem.callbacks.count("hover")) c->setCallback(GuiComponent::CallbackType::HOVER, [=](bool b, glm::vec2 v) {
//        elem.callbacks.at("hover")(b, LuaParser::luaVec(elem.callbacks.at("hover").lua_state(), {v.x, v.y, 0})); });
	
	return c;
}

void GuiBuilder::clearCallbacks(std::shared_ptr<GuiComponent> component) {
	component->setCallback(GuiComponent::CallbackType::PRIMARY, nullptr);
	component->setCallback(GuiComponent::CallbackType::SECONDARY, nullptr);
	component->setCallback(GuiComponent::CallbackType::HOVER, nullptr);
	
	for (auto& child : component->getChildren()) clearCallbacks(child);
}

void GuiBuilder::elementUpdated() {
	dirty = true;
}

GuiBuilder::~GuiBuilder() {
	clear();
}