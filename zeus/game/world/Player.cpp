//
// Created by aurailus on 28/12/18.
//

#include <iostream>
#include "Player.h"
#include "../../engine/Timer.h"

Player::Player() {
    pos = glm::vec3(0, 0, 0);
    vel = glm::vec3(0, 0, 0);
}

void Player::create(World* world, Camera* camera) {
    this->camera = camera;
    this->world = world;
}

void Player::update(bool *keys, double delta, double mouseX, double mouseY) {
    posUpdate(keys, delta);
    viewUpdate(mouseX, mouseY);
    moveCollide();

    camera->setPosition(pos);
}

void Player::posUpdate(bool *keys, double delta) {
    float moveSpeed = 5.0f;
    float jumpVel = 0.14f;
    float friction = 0.3f;

    auto moveMult = (float)(moveSpeed * delta);
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        moveMult *= 2;
    }

    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera->front.x, 0, camera->front.z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));

    auto mod = glm::vec3(0, 0, 0);

    if (keys[GLFW_KEY_W]) mod += frontFlat;
    if (keys[GLFW_KEY_S]) mod -= frontFlat;
    if (keys[GLFW_KEY_D]) mod += rightFlat;
    if (keys[GLFW_KEY_A]) mod -= rightFlat;

    if (glm::length(mod) != 0) mod = glm::normalize(mod);
    mod = mod * moveMult;

    glm::vec3 velFlat = glm::vec3(vel.x, 0, vel.z);
    velFlat = velFlat * (1.0f-friction) + mod * friction;

    vel.x = velFlat.x;
    vel.z = velFlat.z;

    if (keys[GLFW_KEY_SPACE] && collides(glm::vec3(pos.x, pos.y - 0.05, pos.z)) && vel.y >= 0) {
        vel.y = jumpVel;
    }
}

bool Player::collides(glm::vec3 pos) {
    float colSize = 0.4;

    return (world->solidAt(glm::vec3(pos.x - colSize, pos.y - playerHeight, pos.z - colSize)) ||
            world->solidAt(glm::vec3(pos.x + colSize, pos.y - playerHeight, pos.z - colSize)) ||
            world->solidAt(glm::vec3(pos.x + colSize, pos.y - playerHeight, pos.z + colSize)) ||
            world->solidAt(glm::vec3(pos.x - colSize, pos.y - playerHeight, pos.z + colSize)) );
}

void Player::moveCollide() {
    double increment = 0.05;

    Timer t("Move calculations");

    if (!collides(glm::vec3(pos.x, pos.y - increment, pos.z))) {
        vel.y = (float)fmax(vel.y - 0.01, -3);
    }
    else if (vel.y < 0) {
        vel.y = 0;
    }

    double moved = 0;
    for (int i = 0; i < fabs(vel.y) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.y) - moved), 0);
        moved += increment;

        glm::vec3 newPos = glm::vec3(pos);
        newPos.y += move * (vel.y < 0 ? -1 : 1);

        if (!collides(newPos))
            pos = newPos;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.x) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.x) - moved), 0);
        moved += increment;

        glm::vec3 newPos = glm::vec3(pos);
        newPos.x += move * (vel.x < 0 ? -1 : 1);

        if (!collides(newPos))
            pos = newPos;
    }

    moved = 0;
    for (int i = 0; i < fabs(vel.z) / increment; i++) {
        double move = fmax(fmin(increment, fabs(vel.z) - moved), 0);
        moved += increment;

        glm::vec3 newPos = glm::vec3(pos);
        newPos.z += move * (vel.z < 0 ? -1 : 1);

        if (!collides(newPos))
            pos = newPos;
    }

//    t.printElapsedMs();
}

void Player::viewUpdate(double deltaX, double deltaY) {
    float turnSpeed = 0.1f;

    deltaX *= turnSpeed;
    deltaY *= turnSpeed;

    yaw += deltaX;
    pitch += deltaY;

    if (pitch > 90.0f) {
        pitch = 90.0f;
    }
    if (pitch < -90.0f) {
        pitch = -90.0f;
    }

    camera->setYaw(yaw);
    camera->setPitch(pitch);
}

Player::~Player() = default;

glm::vec3 *Player::getPos() {
    return &pos;
}

void Player::setPos(glm::vec3 pos) {
    this->pos = pos;
    camera->setPosition(pos);
}

glm::vec3 *Player::getVel() {
    return &vel;
}

void Player::setVel(glm::vec3 vel) {
    this->vel = vel;
}

float Player::getYaw() {
    return yaw;
}

float Player::getPitch() {
    return pitch;
}
