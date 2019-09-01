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
    model = std::move(model);
}

void Entity::update(double delta) {
    animState.update(delta);

    float factor = static_cast<float>(fmin(delta * 15, 1));

    visualPosition = visualPosition * (1 - factor) + position * factor;
    visualAngle = visualAngle * (1 - factor) + angle * factor;
    visualScale = visualScale * (1 - factor) + scale * factor;
}

void Entity::draw(Renderer& renderer) {
    if (visible) {
        auto mm = getModelMatrix();
        renderer.setModelMatrix(mm);

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

    model = glm::translate(model, visualPosition);
    model = glm::rotate(model, visualAngle * static_cast<float>(3.14159265 / 180), {0.0, 1.0, 0.0});
    model = glm::scale(model, {visualScale, visualScale, visualScale});

    return model;
}

void Entity::cleanup() {
    model = nullptr;
}

Entity::~Entity() {
    cleanup();
}
