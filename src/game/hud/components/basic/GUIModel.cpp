//
// Created by aurailus on 2019-12-17.
//

#include "GUIModel.h"

GUIModel::GUIModel(const std::string &key) : GUIComponent(key) {}

void GUIModel::create(glm::vec2 scale, std::shared_ptr<GuiMesh> model) {
    entity.setMesh(model);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z, scale.x + padding.w + padding.y});
}


void GUIModel::draw(Renderer &renderer) {
    GUIComponent::draw(renderer);
}

void GUIModel::setRotation(glm::mat4 rotation) {
    entity.setRotation(rotation);
}