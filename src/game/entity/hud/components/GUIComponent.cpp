#include <utility>

//
// Created by aurailus on 27/07/19.
//

#include "GUIComponent.h"

GUIComponent::GUIComponent(const std::string& key, GUIComponent *parent) :
    key(key),
    parent(parent) {}

void GUIComponent::setScale(glm::vec2 scale) {
    this->scale = scale;
    entity.setScale({scale.x, scale.y, 1});
}

glm::vec2 GUIComponent::getScale() {
    return scale;
}

void GUIComponent::setPadding(glm::vec4 padding) {
    this->padding = padding;
}

glm::vec4 GUIComponent::getPadding() {
    return padding;
}

void GUIComponent::setPos(glm::vec2 pos) {
    this->pos = pos;
    entity.setPos({pos.x, pos.y, 0});
}

glm::vec2 GUIComponent::getPos() {
    return pos;
}

void GUIComponent::set(std::string key, std::shared_ptr<GUIComponent> component) {
    children[key] = std::move(component);
}

std::shared_ptr<GUIComponent> GUIComponent::operator[](std::string) {
    return children[key];
}

void GUIComponent::remove(std::string key) {
    children.erase(key);
}

void GUIComponent::draw(Renderer& renderer) {
    entity.draw(renderer);
    for (const auto& child : children) {
        child.second->draw(renderer);
    }
}

void GUIComponent::setVisible(bool visible) {
    Drawable::setVisible(visible);
    entity.setVisible(visible);
}