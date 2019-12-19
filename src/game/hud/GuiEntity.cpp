//
// Created by aurailus on 25/11/18.
//

#include "GuiEntity.h"

GuiEntity::GuiEntity(std::shared_ptr<GuiMesh> mesh) {
    setMesh(mesh);
}

void GuiEntity::setMesh(std::shared_ptr<GuiMesh> myMesh) {
    cleanup();
    this->mesh = myMesh;
}

void GuiEntity::draw(Renderer& renderer) {
    if (visible) {
        auto mm = getModelMatrix();
        renderer.setModelMatrix(mm);
        mesh->draw();
    }
}

void GuiEntity::setPos(glm::vec2 position) {
    this->position.x = position.x;
    this->position.y = position.y;
}

glm::vec2 GuiEntity::getPos() {
    return {position.x, position.y};
}

void GuiEntity::setDepth(float depth) {
    position.z = depth;
}

float GuiEntity::getDepth() {
    return position.z;
}

void GuiEntity::setRotation(glm::mat4 rotation){
    this->rotation = rotation;
}

glm::mat4 GuiEntity::getRotation() {
    return rotation;
}

void GuiEntity::setScale(float scale) {
    setScale({scale, scale});
}

void GuiEntity::setScale(glm::vec2 scale) {
    this->scale = {scale.x, scale.y, this->scale.z};
}

void GuiEntity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec2 GuiEntity::getScale() {
    return glm::vec2(scale.x, scale.y);
}

glm::vec3 GuiEntity::getScale3() {
    return scale;
}

glm::mat4 GuiEntity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, position);
    model = model * rotation;
    model = glm::scale(model, scale);

    return model;
}

void GuiEntity::cleanup() {
    mesh = nullptr;
}