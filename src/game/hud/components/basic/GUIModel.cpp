//
// Created by aurailus on 2019-12-17.
//

#include "GUIModel.h"

GUIModel::GUIModel(const std::string &key) : GUIComponent(key) {}

void GUIModel::create(glm::vec2 scale, std::shared_ptr<Model> model) {
    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});

    setRotationX(180);
    setRotationY(215);
}

void GUIModel::update(double delta) {
    entity.update(delta);
}

void GUIModel::animate(glm::vec2 range) {
    entity.playRange(range.x, range.y, true);
}

void GUIModel::setRotationX(float x) {
    entity.setRotateX(x);
}

void GUIModel::setRotationY(float y) {
    entity.setRotateY(y);
}

void GUIModel::setRotationZ(float z) {
    entity.setRotateZ(z);
}

void GUIModel::draw(Renderer &renderer) {
    renderer.toggleDepthTest(true);
    renderer.clearDepthBuffer();
    GUIComponent::draw(renderer);
    renderer.toggleDepthTest(false);
}