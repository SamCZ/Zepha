//
// Created by aurailus on 25/11/18.
//

#include "GuiEntity.h"

GuiEntity::GuiEntity(GuiMesh* mesh) {
    setMesh(mesh);
}

void GuiEntity::setMesh(GuiMesh* myMesh) {
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
    this->position = position;
}

glm::vec2 GuiEntity::getPos() {
    return position;
}

void GuiEntity::setAngle(float angle) {
    this->angle = angle;
}

float GuiEntity::getAngle() {
    return angle;
}

void GuiEntity::setScale(float scale) {
    setScale({scale, scale});
}

void GuiEntity::setScale(glm::vec2 scale) {
    this->scale = scale;
}

glm::vec2 GuiEntity::getScale() {
    return scale;
}

glm::mat4 GuiEntity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, {position.x, position.y, 0});
    model = glm::rotate(model, angle * (GLfloat)(3.14159265 / 180), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, {scale.x, scale.y, 1});

    return model;
}

void GuiEntity::cleanup() {
    delete mesh;
    mesh = nullptr;
}

GuiEntity::~GuiEntity() {
    cleanup();
}