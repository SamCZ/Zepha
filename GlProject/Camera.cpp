//
// Created by aurailus on 27/11/18.
//

#include "Camera.h"
#include <stdio.h>

//Camera::Camera() {}

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->moveSpeed = moveSpeed;
    this->turnSpeed = turnSpeed;

    update();
}

void Camera::keyControl(bool *keys, GLfloat delta) {
    GLfloat moveMult = moveSpeed * delta;

    if (keys[GLFW_KEY_W]) {
        position += front * moveMult;
    }

    if (keys[GLFW_KEY_S]) {
        position -= front * moveMult;
    }

    if (keys[GLFW_KEY_D]) {
        position += right * moveMult;
    }

    if (keys[GLFW_KEY_A]) {
        position -= right * moveMult;
    }
}

void Camera::mouseControl(double deltaX, double deltaY) {

    deltaX *= turnSpeed;
    deltaY *= turnSpeed;
//
//    if (deltaX != 0 || deltaY != 0) {
//        printf("%f, %f\n", deltaX, deltaY);
//    }
//
    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::calculateViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::update() {
    front.x = (float)(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front.y = (float)(sin(glm::radians(pitch)));
    front.z = (float)(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}
