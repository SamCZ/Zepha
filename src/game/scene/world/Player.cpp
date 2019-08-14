//
// Created by aurailus on 28/12/18.
//

#include "Player.h"
#include "../../../util/Ray.h"

Player::Player(LocalWorld& world, LocalDefs& defs, Camera& camera) :
    world(world),
    camera(camera),
    defs(defs),
    wireframe(WireframeEntity({}, 0.01, {1, 1, 1})) {}

void Player::update(InputManager &input, double delta, double mouseX, double mouseY) {
    posUpdate(input, delta);
    viewUpdate(mouseX, mouseY);
    pointerUpdate(input, delta);
    moveCollide();
}

void Player::posUpdate(InputManager &input, double delta) {
    const static float moveSpeed = 7.5f;
    const static float jumpVel = 0.14f;

    float friction = 0.3f;

    double moveMult = moveSpeed * delta;

    if (input.isKeyPressed(GLFW_KEY_F)) {
        flying = !flying;
    }

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

    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera.getFront().x, 0, camera.getFront().z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera.getRight().x, 0, camera.getRight().z));

    glm::vec3 mod(0, 0, 0);

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

void Player::viewUpdate(double deltaX, double deltaY) {
    float turnSpeed = 0.1f;

    deltaX *= turnSpeed;
    deltaY *= turnSpeed;

    yaw += deltaX;
    pitch += deltaY;

    if (yaw > 360.f) yaw = 0;
    if (yaw < 0.f)   yaw = 360.f;

    if (pitch > 90.f)  pitch = 90.f;
    if (pitch < -90.f) pitch = -90.f;

    camera.setYaw(yaw);
    camera.setPitch(pitch);
    camera.setPos(pos);
}

void Player::pointerUpdate(InputManager &input, double delta) {
    bool found = false;

    for (Ray ray(this); ray.getLength() < Player::LOOK_DISTANCE; ray.step(LOOK_PRECISION)) {
        auto rayEnd = *ray.getEnd();
        auto pointedPos = TransPos::roundPos(rayEnd);

        auto blockID = world.getBlock(rayEnd);
        auto& sBoxes = defs.defs().blockFromId(blockID).sBoxes;

        for (auto& sBox : sBoxes) {
            auto intersects = sBox.intersects(*ray.getEnd(), pointedPos);

            if (intersects != NONE) {
                pointedThing.blockID = blockID;
                pointedThing.blockDef = &defs.defs().blockFromId(blockID);
                pointedThing.pos = pointedPos;
                pointedThing.face = intersects;

                wireframe.updateMesh(sBoxes, 0.000005f + ray.getLength() * 0.0015f);
                wireframe.setPos(pointedPos);
                if (!wireframe.isVisible()) wireframe.setVisible(true);

                found = true;
                goto stop;
            }
        }
    }
    stop:

    if (found) {
        const static float DAMAGE = 0.25f;
        const static float INTERVAL = 0.1f;

        if (input.isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
            if (breakInterval == 0) {
                world.damageBlock(pointedThing.pos, DAMAGE);
            }
            breakInterval += delta;
            if (breakInterval > INTERVAL) breakInterval = 0;
        }
        else {
            if (breakInterval > 0) breakInterval += delta;
            if (breakInterval > INTERVAL) breakInterval = 0;
        }
        if (input.isMousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            world.localSetBlock(pointedThing.pos + SelectionBox::faceToOffset(pointedThing.face),
                                defs.defs().blockFromStr("default:grass").index);
        }
    }
    else {
        if (wireframe.isVisible()) wireframe.setVisible(false);
        breakInterval = 0;
    }
}

bool Player::collides(glm::vec3 pos) {
    float colSize = 0.3;

    return (world.solidAt(glm::vec3(pos.x - colSize, pos.y - EYE_HEIGHT, pos.z - colSize)) ||
            world.solidAt(glm::vec3(pos.x + colSize, pos.y - EYE_HEIGHT, pos.z - colSize)) ||
            world.solidAt(glm::vec3(pos.x + colSize, pos.y - EYE_HEIGHT, pos.z + colSize)) ||
            world.solidAt(glm::vec3(pos.x - colSize, pos.y - EYE_HEIGHT, pos.z + colSize)) );
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

glm::vec3 Player::getPos() {
    return pos;
}

void Player::setPos(glm::vec3 pos) {
    this->pos = pos;
    camera.setPos(pos);
}

glm::vec3 Player::getVel() {
    return vel;
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

PointedThing& Player::getPointedThing() {
    return pointedThing;
}

void Player::draw(Renderer &renderer) {
    wireframe.draw(renderer);
}
