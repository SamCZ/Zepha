//
// Created by aurailus on 27/07/19.
//

#include <utility>
#include "GuiComponent.h"

GuiComponent::GuiComponent(const std::string& key) :
    key(key) {}

void GuiComponent::setScale(glm::vec2 scale) {
    this->scale = scale;
    entity.setScale({scale.x, scale.y, scale.x});
}

glm::vec2 GuiComponent::getScale() {
    return scale;
}

void GuiComponent::setPadding(glm::vec4 padding) {
    this->padding = padding;
}

glm::vec4 GuiComponent::getPadding() {
    return padding;
}

void GuiComponent::setPos(glm::ivec2 pos) {
    this->pos = pos;
    if (parent != nullptr) {
        glm::vec3 parentPos = parent->entity.getPos();
        pos += glm::vec2 {parentPos.x, parentPos.y};
        pos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }
    entity.setPos({pos.x, pos.y, depth});
    for (const auto& child : children) {
        child->updatePos();
    }
}

glm::ivec2 GuiComponent::getPos() {
    return pos;
}

void GuiComponent::add(std::shared_ptr<GuiComponent> component) {
    component->parent = this;
    component->updatePos();
    children.push_back(std::move(component));
}

void GuiComponent::insert(unsigned int index, std::shared_ptr<GuiComponent> component) {
    component->parent = this;
    component->updatePos();
    children.insert(std::next(children.begin(), index), std::move(component));
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
        child->draw(renderer);
    }
}

void GuiComponent::setVisible(bool visible) {
    Drawable::setVisible(visible);
    entity.setVisible(visible);
    for (const auto& child : children) {
        child->setVisible(visible);
    }
}

void GuiComponent::updatePos() {
    glm::vec2 realPos(pos);
    if (parent != nullptr) {
        glm::vec3 parentPos = parent->entity.getPos();
        realPos += glm::vec2 {parentPos.x, parentPos.y};
        realPos += glm::vec2 {parent->getPadding().w, parent->getPadding().x};
    }
    entity.setPos({realPos.x, realPos.y, depth});
    for (const auto& child : children) {
        child->updatePos();
    }
}

bool GuiComponent::mouseActivity(glm::ivec2 pos) {
    bool isHovering = false;
    for (auto child = children.rbegin(); child != children.rend(); ++child) {
        if ((*child)->mouseActivity(pos - (*child)->getPos() - glm::ivec2((*child)->getPadding().y, (*child)->getPadding().x))) isHovering = true;
    }
    if (pos.x >= 0 && pos.y >= 0 && pos.x <= hitbox.x && pos.y <= hitbox.y) {
        if (cbHover) {
            cbHover(true, pos);
            hovered = true;
            return true;
        }
        return isHovering;
    }
    else {
        if (cbHover) {
            cbHover(false, pos);
            hovered = false;
        }
    }
    return isHovering;
}

void GuiComponent::update(double delta) {
    for (const auto& child : children) {
        child->update(delta);
    }
}

bool GuiComponent::leftClickEvent(bool state, glm::ivec2 pos) {
    clickEvent(true, state, pos);
}

bool GuiComponent::rightClickEvent(bool state, glm::ivec2 pos) {
    clickEvent(false, state, pos);
}

void GuiComponent::setHoverCallback(const callback& hoverCallback) {
    cbHover = hoverCallback;
}

void GuiComponent::setLeftClickCallback(const callback& leftClickCallback) {
    cbLeftClick = leftClickCallback;
}

void GuiComponent::setRightClickCallback(const callback& rightClickCallback) {
    cbRightClick = rightClickCallback;
}

void GuiComponent::setCallbacks(const callback &left, const callback &right, const callback &hover) {
    setLeftClickCallback(left);
    setRightClickCallback(right);
    setHoverCallback(hover);
}

const std::string &GuiComponent::getKey() {
    return key;
}

bool GuiComponent::clickEvent(bool left, bool state, glm::ivec2 pos) {
    for (auto child = children.rbegin(); child != children.rend(); ++child) {
        glm::ivec2 cp = pos - (*child)->getPos() - glm::ivec2((*child)->getPadding().y, (*child)->getPadding().x);
        if ((*child)->clickEvent(left, state, cp)) return true;
    }
    if (pos.x >= 0 && pos.y >= 0 && pos.x <= hitbox.x && pos.y <= hitbox.y && (left ? cbLeftClick : cbRightClick)) {
        (left ? cbLeftClick : cbRightClick)(state, pos);
        return true;
    }
    return false;
}