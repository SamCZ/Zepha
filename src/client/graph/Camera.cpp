//
// Created by aurailus on 27/11/18.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

Camera::Camera() = default;

void Camera::create(float buffWidth, float buffHeight, glm::vec3 up) {
	this->bufferDimensions = glm::vec2(buffWidth, buffHeight);
	
	this->position = glm::vec3(0, 0, 0);
	
	this->worldUp = up;
	this->yaw = 0;
	this->pitch = 0;
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	
	createMatrices();
}

void Camera::createMatrices() {
	ratio = bufferDimensions.x / bufferDimensions.y;
	
	projectionMatrix = glm::perspective(glm::radians(fov), ratio, nearClip, farClip);
	frustum.setCamInternals(glm::radians(fov) / 1.2f, ratio, nearClip, farClip);
	
	orthographicMatrix = glm::ortho(0.0f, bufferDimensions.x, bufferDimensions.y, 0.0f, -1000.0f, 1000.0f);
}

void Camera::changeWindowDimensions(glm::vec2 size) {
	this->bufferDimensions = size;
	createMatrices();
}

glm::mat4 Camera::getProjectionMatrix() {
	return projectionMatrix;
}

glm::mat4 Camera::getOrthographicMatrix() {
	return orthographicMatrix;
}

glm::mat4 Camera::getViewMatrix() {
	update();
	return glm::lookAt(position, position + front, up);
}

void Camera::setPos(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Camera::getPos() {
	return position;
}

void Camera::setYaw(double yaw) {
	this->yaw = yaw;
}

void Camera::setPitch(double pitch) {
	this->pitch = pitch;
}

void Camera::update() {
	front.x = (float) (cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front.y = (float) (sin(glm::radians(pitch)));
	front.z = (float) (sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
	front = glm::normalize(front);
	
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	
	frustum.update(position, front, up, right);
}

glm::vec3 Camera::getFront() {
	return front;
}

glm::vec3 Camera::getRight() {
	return right;
}

glm::vec2 Camera::getBufferDimensions() {
	return bufferDimensions;
}

bool Camera::inFrustum(glm::vec3& p) {
	return frustum.pointInFrustum(p) == Frustum::Intersection::INSIDE;
}

Frustum::Intersection Camera::inFrustum(FrustumAABB& b) {
	return frustum.boxInFrustum(b);
}

Camera::~Camera() = default;
