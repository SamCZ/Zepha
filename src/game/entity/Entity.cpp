#include <utility>

//
// Created by aurailus on 25/11/18.
//

#include "Entity.h"

Entity::Entity() {
    model = std::make_unique<Model>();
};

Entity::Entity(std::shared_ptr<Model> model) : animState(*model), model(model) {}

void Entity::setModel(std::shared_ptr<Model> model) {
    animState = AnimationState(*model);
    this->model = std::move(model);
    animState.setPlaying(true);
}

void Entity::update(double delta) {
    animState.update(delta);

    if (animState.getFrame() > 390) animState.setFrame(302);

    float factor = static_cast<float>(fmin(delta * 8, 1));

    visualPosition = visualPosition * (1 - factor) + position * factor;
    visualVisualOffset = visualVisualOffset * (1 - factor) + visualOffset * factor;
    visualAngle = visualAngle * (1 - factor) + angle * factor;
    visualScale = visualScale * (1 - factor) + scale * factor;
}

void Entity::draw(Renderer& renderer) {
    if (visible) {
        renderer.setModelMatrix(getModelMatrix());

        model->getTransformsByFrame(animState.getFrame(), animState.getBounds(), transforms);
        renderer.setBones(transforms);

        for (const auto& mesh : model->meshes) mesh->draw();
    }
}

void Entity::setPos(glm::vec3 position) {
    this->visualPosition = position;
    this->position = position;
}

void Entity::interpPos(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Entity::getPos() {
    return position;
}

void Entity::setVisualOffset(glm::vec3 vs) {
    this->visualVisualOffset = vs;
    this->visualOffset = vs;
}

void Entity::interpVisualOffset(glm::vec3 vs){
    this->visualOffset = vs;
}

glm::vec3 Entity::getVisualOffset(){
    return visualOffset;
}

void Entity::setAngle(float angle) {
    this->visualAngle = angle;
    this->angle = angle;
}

void Entity::interpAngle(float angle) {
    this->angle = angle;
}

float Entity::getAngle() {
    return angle;
}

void Entity::setRotation(glm::mat4 rotation) {
    this->rotation = rotation;
}

glm::mat4 Entity::getRotation() {
    return rotation;
}

void Entity::setScale(float scale) {
    this->visualScale = scale;
    this->scale = scale;
}

void Entity::interpScale(float scale) {
    this->scale = scale;
}

float Entity::getScale() {
    return scale;
}

glm::mat4 Entity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, visualPosition + visualVisualOffset);
//    model = glm::rotate(model, visualAngle * static_cast<float>(3.14159265 / 180), {0.0, 1.0, 0.0});
    model = model * rotation;
    model = glm::scale(model, glm::vec3(visualScale));

    return model;
}

void Entity::cleanup() {
    model = nullptr;
}

Entity::~Entity() {
    cleanup();
}
