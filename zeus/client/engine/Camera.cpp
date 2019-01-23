//
// Created by aurailus on 27/11/18.
//

#include "Camera.h"

Camera::Camera() = default;

void Camera::create(glm::vec3 up) {
    this->position = glm::vec3(0, 0, 0);
    this->worldUp = up;
    this->yaw = 0;
    this->pitch = 0;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);

    update();
}

glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::setYaw(double yaw) {
    this->yaw = yaw;

    update();
}

void Camera::setPitch(double pitch) {
    this->pitch = pitch;

    update();
}

void Camera::update() {
    front.x = (float)(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = (float)(sin(glm::radians(pitch)));
    front.z = (float)(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 *Camera::getPosition() {
    return &position;
}

void Camera::setPosition(glm::vec3 pos) {
    position = pos;
}

Camera::~Camera() = default;
