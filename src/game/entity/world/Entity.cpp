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
    this->position = position;
}

glm::vec3 Entity::getPos() {
    return position;
}

void Entity::setAngle(float angle) {
    this->angle = angle;
}

float Entity::getAngle() {
    return angle;
}

void Entity::setScale(float scale) {
    setScale(glm::vec3(scale, scale, scale));
}

void Entity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec3 Entity::getScale() {
    return scale;
}

glm::mat4 Entity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, position);
    model = glm::rotate(model, angle * static_cast<float>(3.14159265 / 180), {0.0, 1.0, 0.0});
    model = glm::scale(model, scale);

    return model;
}

void Entity::cleanup() {
    model = nullptr;
}

Entity::~Entity() {
    cleanup();
}