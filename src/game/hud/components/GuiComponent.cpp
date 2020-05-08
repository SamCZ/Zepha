//
// Created by aurailus on 27/07/19.
//

#include <utility>
#include "GuiComponent.h"

GuiComponent::GuiComponent(const std::string& key) :
    key(key) {}

void GuiComponent::update(double delta) {
    for (const auto& child : children) {
        child->update(delta);
    }
}

const std::string& GuiComponent::getKey() {
    return key;
}

glm::ivec2 GuiComponent::getPos() {
    return pos;
}

void GuiComponent::setPos(glm::ivec2 pos) {
    this->pos = pos;

    if (parent != nullptr) {
        glm::vec3 parentPos = parent->entity.getPos();
        pos += glm::vec2 {parentPos.x, parentPos.y};
        pos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }

    entity.setPos({pos.x, pos.y, 0});

    for (const auto& child : children) {
        child->updatePos();
    }
}

glm::vec2 GuiComponent::getScale() {
    return scale;
}

void GuiComponent::setScale(glm::vec2 scale) {
    this->scale = scale;
    entity.setScale({scale.x, scale.y, scale.x});
}

glm::vec4 GuiComponent::getPadding() {
    return padding;
}

void GuiComponent::setPadding(glm::vec4 padding) {
    this->padding = padding;
}

void GuiComponent::setOverflows(bool overflows) {
    this->overflows = overflows;
}

void GuiComponent::setVisible(bool visible) {
    Drawable::setVisible(visible);
    entity.setVisible(visible);
    for (const auto& child : children) {
        child->setVisible(visible);
    }
}

void GuiComponent::setCallback(GuiComponent::CallbackType type, const callback& cb) {
    callbacks[static_cast<unsigned int>(type)] = cb;
}

void GuiComponent::handleMouseInput(Window &window) {
    auto mousePos = window.input.mousePos();

    window.setCursorHand(mouseActivity(mousePos));
    if (window.input.mousePressed(GLFW_MOUSE_BUTTON_LEFT))   clickEvent(true,  true,  mousePos);
    if (window.input.mouseReleased(GLFW_MOUSE_BUTTON_LEFT))  clickEvent(true,  false, mousePos);
    if (window.input.mousePressed(GLFW_MOUSE_BUTTON_RIGHT))  clickEvent(false, true,  mousePos);
    if (window.input.mouseReleased(GLFW_MOUSE_BUTTON_RIGHT)) clickEvent(false, false, mousePos);
}

bool GuiComponent::mouseActivity(glm::ivec2 pos) {
    bool isHovering = false;
    for (auto& child : children) {
        glm::ivec2 cp = pos - child->getPos() - glm::ivec2(child->getPadding().y, child->getPadding().x);
        if (child->mouseActivity(cp)) isHovering = true;
    }

    auto& callback = callbacks[static_cast<unsigned int>(CallbackType::HOVER)];

    if (pos.x >= 0 && pos.y >= 0 && pos.x <= hitbox.x && pos.y <= hitbox.y) {
        if (callback) {
            callback(true, pos);
            hovered = true;
            return true;
        }
        return isHovering;
    }
    else {
        if (callback) {
            callback(false, pos);
            hovered = false;
        }
    }
    return isHovering;
}

std::shared_ptr<GuiComponent> GuiComponent::insert(unsigned int index, std::shared_ptr<GuiComponent> component) {
    component->parent = this;
    component->updatePos();
    children.insert(std::next(children.begin(), index), std::move(component));
    return component;
}

std::shared_ptr<GuiComponent> GuiComponent::add(std::shared_ptr<GuiComponent> component) {
    component->parent = this;
    component->updatePos();
    children.push_back(std::move(component));
    return component;
}

void GuiComponent::remove(const std::string& key) {
    for (auto it = children.cbegin(); it != children.cend(); it++) {
        if (it->get()->key == key) {
            children.erase(it);
            return;
        }
    }
}

void GuiComponent::empty() {
    for (auto it = children.cbegin(); it != children.cend();) {
        it = children.erase(it);
    }
}

void GuiComponent::draw(Renderer& renderer) {
    entity.draw(renderer);

    for (const auto& child : children) {
        renderer.setClipBounds(overflows ? glm::vec4 {} : glm::vec4 {entity.getPos().x, entity.getPos().y,
            entity.getPos().x + scale.x, entity.getPos().y + scale.y});
        child->draw(renderer);
    }
}

bool GuiComponent::clickEvent(bool left, bool state, glm::ivec2 pos) {
    for (auto& child : children) {
        glm::ivec2 cp = pos - child->getPos() - glm::ivec2(child->getPadding().y, child->getPadding().x);
        if (child->clickEvent(left, state, cp)) return true;
    }

    auto& callback = callbacks[static_cast<unsigned int>(left ? CallbackType::PRIMARY : CallbackType::SECONDARY)];

    if (pos.x >= 0 && pos.y >= 0 && pos.x <= hitbox.x && pos.y <= hitbox.y && callback) {
        callback(state, pos);
        return true;
    }
    return false;
}

void GuiComponent::updatePos() {
    glm::vec2 realPos(pos);
    if (parent != nullptr) {
        glm::vec3 parentPos = parent->entity.getPos();
        realPos += glm::vec2 {parentPos.x, parentPos.y};
        realPos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }
    entity.setPos({realPos.x, realPos.y, 0});
    for (const auto& child : children) {
        child->updatePos();
    }
}

std::list<std::shared_ptr<GuiComponent>> GuiComponent::getChildren() {
    return children;
}
