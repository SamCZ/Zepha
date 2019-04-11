//
// Created by aurailus on 28/12/18.
//

#include "Player.h"
#include "../engine/Ray.h"

Player::Player() {
    pos = glm::vec3(0, 0, 0);
    vel = glm::vec3(0, 0, 0);
    pointedBlock = glm::vec3(0, 0, 0);
    flying = false;
    digPercentage = 0;
    pointingAtBlock = false;
}

void Player::create(LocalWorld* world, Camera* camera, WireframeEntity* wireframe) {
    this->camera = camera;
    this->world = world;
    this->wireframe = wireframe;
}

void Player::update(InputManager &input, double delta, double mouseX, double mouseY) {
    posUpdate(input, delta);
    viewUpdate(mouseX, mouseY);
    moveCollide();

    bool found = false;
    glm::vec3 foundPos {};

    for (Ray ray(this); ray.getLength() < Player::LOOK_DISTANCE; ray.step(0.01)) {
        auto rayEnd = *ray.getEnd();
        auto pointedAt = TransPos::roundPos(rayEnd);

        auto at = world->getBlock(rayEnd);
        if (at > 0) {
            auto sBox = world->getBlockAtlas()->getBlock(at)->getSelectionBox();

            if (rayEnd.x >= pointedAt.x + sBox.a.x && rayEnd.y >= pointedAt.y + sBox.a.y && rayEnd.z >= pointedAt.z + sBox.a.z &&
                rayEnd.x <= pointedAt.x + sBox.b.x && rayEnd.y <= pointedAt.y + sBox.b.y && rayEnd.z <= pointedAt.z + sBox.b.z) {

                if (sBox != box) {
                    box = sBox;
                }

                wireframe->updateMesh(box.a, box.b, 0.003f + ray.getLength() * 0.002f, glm::vec3(0.1));
                wireframe->setPos(pointedAt);

                if (!wireframe->isVisible()) wireframe->setVisible(true);

                found = true;
                foundPos = pointedAt;
                break;
            }
        }
    }

    if (found) {
        pointingAtBlock = true;

        if (foundPos != pointedBlock) {
            pointedBlock = foundPos;
            digPercentage = 0;
        }

        if (input.isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
            digPercentage += delta * 30;
        }

        if (digPercentage >= 1) {
            world->setBlock(pointedBlock, 0);
            digPercentage = 0;
        }
    }
    else {
        pointingAtBlock = false;
        digPercentage = 0;

        if (wireframe->isVisible()) {
            wireframe->setVisible(false);
        }
    }

    if (input.isKeyPressed(GLFW_KEY_F)) {
        flying = !flying;
    }

    camera->setPosition(pos);
}

void Player::posUpdate(InputManager &input, double delta) {
    float moveSpeed = 7.5f;
    float jumpVel = 0.14f;
    float friction = 0.3f;

    double moveMult = moveSpeed * delta;


    if (flying) {
        moveMult *= 4;
        friction = 0.15f;

        if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
            moveMult *= 2;
        }
    }
    else {
        if (input.isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
            moveMult *= 1.5;
        }
    }

    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera->getFront()->x, 0, camera->getFront()->z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera->getRight()->x, 0, camera->getRight()->z));

    auto mod = glm::vec3(0, 0, 0);

    if (input.isKeyDown(GLFW_KEY_W)) mod += frontFlat;
    if (input.isKeyDown(GLFW_KEY_S)) mod -= frontFlat;
    if (input.isKeyDown(GLFW_KEY_D)) mod += rightFlat;
    if (input.isKeyDown(GLFW_KEY_A)) mod -= rightFlat;

    if (flying) {
        if (input.isKeyDown(GLFW_KEY_SPACE)) mod.y += 1;
        if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) mod.y -= 1;
    }

    if (glm::length(mod) != 0) mod = glm::normalize(mod);
    mod = mod * (float)moveMult;

    if (!flying) {
        glm::vec3 velFlat = glm::vec3(vel.x, 0, vel.z);
        velFlat = velFlat * (1.0f-friction) + mod * friction;

        vel.x = velFlat.x;
        vel.z = velFlat.z;

        if (input.isKeyDown(GLFW_KEY_SPACE) && collides(glm::vec3(pos.x, pos.y - 0.05, pos.z)) && vel.y >= 0) {
            vel.y = jumpVel;
        }
    }
    else {
        vel = vel * (1.0f-friction) + mod * friction;
    }
}

bool Player::collides(glm::vec3 pos) {
    float colSize = 0.3;

    return (world->solidAt(glm::vec3(pos.x - colSize, pos.y - EYE_HEIGHT, pos.z - colSize)) ||
            world->solidAt(glm::vec3(pos.x + colSize, pos.y - EYE_HEIGHT, pos.z - colSize)) ||
            world->solidAt(glm::vec3(pos.x + colSize, pos.y - EYE_HEIGHT, pos.z + colSize)) ||
            world->solidAt(glm::vec3(pos.x - colSize, pos.y - EYE_HEIGHT, pos.z + colSize)) );
}

void Player::moveCollide() {
    if (!flying) {
        double increment = 0.05;

        if (!collides(glm::vec3(pos.x, pos.y - increment, pos.z))) {
            vel.y = (float) fmax(vel.y - 0.01, -3);
        } else if (vel.y < 0) {
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
    }
    else {
        pos += vel;
    }
}

void Player::viewUpdate(double deltaX, double deltaY) {
    float turnSpeed = 0.1f;

    deltaX *= turnSpeed;
    deltaY *= turnSpeed;

    yaw += deltaX;
    pitch += deltaY;

    if (yaw > 360.0f) {
        yaw = 0;
    }
    if (yaw < 0) {
        yaw = 360.0f;
    }
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

glm::vec3* Player::getPointedBlock() {
    return (pointingAtBlock) ? &pointedBlock : nullptr;
}
