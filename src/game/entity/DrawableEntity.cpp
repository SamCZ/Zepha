//
// Created by aurailus on 25/11/18.
//

#include <utility>
#include <glm/gtc/matrix_transform.hpp>

#include "DrawableEntity.h"

#include "Model.h"
#include "AnimationSegment.h"
#include "../graph/Renderer.h"
#include "../graph/meshtypes/EntityMesh.h"

DrawableEntity::DrawableEntity() : model(std::make_shared<Model>()) {}

DrawableEntity::DrawableEntity(std::shared_ptr<Model> model) :
    animState(*model),
    model(model) {}

void DrawableEntity::setModel(std::shared_ptr<Model> model) {
    animState = AnimationState(*model);
    this->model = std::move(model);
    animState.setPlaying(true);
}

void DrawableEntity::update(double delta) {
    animState.update(delta);

    float factor = static_cast<float>(fmin(delta * 8, 1));

    visualPosition = visualPosition * (1 - factor) + pos * factor;
    visualVisualOffset = visualVisualOffset * (1 - factor) + visualOff * factor;
    visualRotation = visualRotation * (1 - factor) + rot * factor;
    visualScale = visualScale * (1 - factor) + scale * factor;
}

void DrawableEntity::setPos(glm::vec3 position) {
    visualPosition = position;
    Entity::setPos(position);
}

void DrawableEntity::interpPos(glm::vec3 position) {
    Entity::setPos(position);
}

void DrawableEntity::setVisualOffset(glm::vec3 vs) {
    visualVisualOffset = vs;
    Entity::setVisualOffset(vs);
}

void DrawableEntity::interpVisualOffset(glm::vec3 vs) {
    Entity::setVisualOffset(vs);
}

void DrawableEntity::setRotateX(float rotation) {
    visualRotation.x = rotation;
    Entity::setRotateX(rotation);
}

void DrawableEntity::interpRotateX(float rotation) {
    Entity::setRotateX(rotation);
}

void DrawableEntity::setRotateY(float rotation) {
    visualRotation.y = rotation;
    Entity::setRotateY(rotation);
}

void DrawableEntity::interpRotateY(float rotation) {
    Entity::setRotateY(rotation);
}

void DrawableEntity::setRotateZ(float rotation) {
    visualRotation.z = rotation;
    Entity::setRotateZ(rotation);

}

void DrawableEntity::interpRotateZ(float rotation) {
    Entity::setRotateZ(rotation);
}

void DrawableEntity::setScale(float scale) {
    visualScale = glm::vec3(scale);
    Entity::setScale(scale);
}

void DrawableEntity::interpScale(float scale) {
    Entity::setScale(scale);
}

void DrawableEntity::setScale(glm::vec3 scale) {
    visualScale = scale;
    Entity::setScale(scale);

}

void DrawableEntity::interpScale(glm::vec3 scale) {
    Entity::setScale(scale);
}

void DrawableEntity::setAnimations(const std::vector<AnimationSegment>& anims) {
    animState.setAnimations(anims);
}

void DrawableEntity::playAnimation(const std::string &anim, bool loop) {
    animState.setAnimNamed(anim, 0, loop);
}

void DrawableEntity::playRange(unsigned int start, unsigned int end, bool loop) {
    animState.setAnimRange(start, end, 0, loop);
}

void DrawableEntity::setPlaying(bool playing, unsigned int offset) {
    animState.setPlaying(playing);
    if (offset != UINT_MAX) animState.setFrame(offset + animState.getFrame());
}

void DrawableEntity::draw(Renderer& renderer) {
    if (visible) {
        renderer.setModelMatrix(getModelMatrix());

        model->getTransformsByFrame(animState.getFrame(), animState.getBounds(), transforms);
        renderer.setBones(transforms);

        for (const auto& mesh : model->meshes) mesh->draw();
    }
}

glm::mat4 DrawableEntity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, visualPosition + visualVisualOffset);
    model = model * getRotationMatrix();
    model = glm::scale(model, visualScale);

    return model;
}

glm::mat4 DrawableEntity::getRotationMatrix() {
    glm::mat4 parentMatrix = (parent != nullptr ? parent->getRotationMatrix() : glm::mat4(1.0f));
    glm::mat4 rotMatrix = glm::mat4(1.0f);
    rotMatrix = glm::rotate(rotMatrix, glm::radians(visualRotation.x), {1, 0, 0});
    rotMatrix = glm::rotate(rotMatrix, glm::radians(visualRotation.y), {0, 1, 0});
    rotMatrix = glm::rotate(rotMatrix, glm::radians(visualRotation.z), {0, 0, 1});
    return parentMatrix * rotMatrix;
}


void DrawableEntity::cleanup() {
    model = nullptr;
}

DrawableEntity::~DrawableEntity() {
    cleanup();
}