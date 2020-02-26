#include <utility>

//
// Created by aurailus on 25/11/18.
//

#include "Entity.h"

Entity::Entity() : model(std::make_unique<Model>()) {}

Entity::Entity(std::shared_ptr<Model> model) : animState(*model), model(model) {}

void Entity::setModel(std::shared_ptr<Model> model) {
    animState = AnimationState(*model);
    this->model = std::move(model);
    animState.setPlaying(true);
}

void Entity::update(double delta) {
    animState.update(delta);

    float factor = static_cast<float>(fmin(delta * 8, 1));

    visualPosition = visualPosition * (1 - factor) + position * factor;
    visualVisualOffset = visualVisualOffset * (1 - factor) + visualOffset * factor;
    visualRotation = visualRotation * (1 - factor) + rotation * factor;
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

void Entity::setRotateX(float rotation) {
    this->visualRotation.x = rotation;
    this->rotation.x = rotation;
}

void Entity::interpRotateX(float rotation) {
    this->rotation.x = rotation;
}

float Entity::getRotateX() {
    return rotation.x;
}

void Entity::setRotateY(float rotation) {
    this->visualRotation.y = rotation;
    this->rotation.y = rotation;
}

void Entity::interpRotateY(float rotation) {
    this->rotation.y = rotation;
}

float Entity::getRotateY() {
    return rotation.y;
}

void Entity::setRotateZ(float rotation) {
    this->visualRotation.z = rotation;
    this->rotation.z = rotation;
}

void Entity::interpRotateZ(float rotation) {
    this->rotation.z = rotation;
}

float Entity::getRotateZ() {
    return rotation.z;
}

void Entity::setScale(float scale) {
    this->visualScale = glm::vec3(scale);
    this->scale = glm::vec3(scale);
}

void Entity::interpScale(float scale) {
    this->scale = glm::vec3(scale);
}

void Entity::setScale(glm::vec3 scale) {
    this->visualScale = scale;
    this->scale = scale;
}

void Entity::interpScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec3 Entity::getScale() {
    return scale;
}

glm::mat4 Entity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, visualPosition + visualVisualOffset);

    model = glm::rotate(model, glm::radians(visualRotation.x), {1, 0, 0});
    model = glm::rotate(model, glm::radians(visualRotation.y), {0, 1, 0});
    model = glm::rotate(model, glm::radians(visualRotation.z), {0, 0, 1});

    model = glm::scale(model, visualScale);

    return model;
}

void Entity::cleanup() {
    model = nullptr;
}

Entity::~Entity() {
    cleanup();
}

void Entity::setAnimations(const std::vector<AnimationSegment>& anims) {
    animState.setAnimations(anims);
}

void Entity::playAnimation(const std::string &anim, bool loop) {
    animState.setAnimNamed(anim, 0, loop);
}

void Entity::playRange(unsigned int start, unsigned int end, bool loop) {
    animState.setAnimRange(start, end, 0, loop);
}

void Entity::setPlaying(bool playing, unsigned int offset) {
    animState.setPlaying(playing);
    if (offset != UINT_MAX) animState.setFrame(offset + animState.getFrame());
}