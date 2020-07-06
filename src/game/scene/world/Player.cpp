//
// Created by aurailus on 28/12/18.
//

#include "Player.h"

#include "../../../util/Ray.h"
#include "../../graph/Renderer.h"
#include "../../../world/chunk/Chunk.h"

Player::Player(LocalWorld& world, ClientGame& defs, Renderer& renderer, LocalInventoryRefs& refs) :
    Collidable(world, defs, {{-0.3, 0, -0.3}, {0.3, 1.8, 0.3}}),

    refs(refs),
    game(defs),
    renderer(renderer),
    wireframe({}, 0.01, {1, 1, 1}),
    gameGui(refs, renderer.window.getSize(), defs, renderer) {
    handItemModel.parent = &handModel;

    renderer.window.addResizeCallback("player", [&](glm::ivec2 win) {
        gameGui.winResized(win);
    });
}

void Player::update(Input &input, double delta, glm::vec2 mouseDelta) {
    gameGui.update(delta);
    handItemModel.setVisible(gameGui.isVisible());

    moveAndLook(input, delta, mouseDelta);
    findPointedThing(input);
    updateWireframe();
    breakBlock(input, delta);
}

void Player::moveAndLook(Input &input, double delta, glm::vec2 mouseDelta) {

    //Position movement
    bool sprinting = input.keyDown(GLFW_KEY_LEFT_CONTROL);

    double moveSpeed = BASE_MOVE_SPEED * delta * (sprinting ? 1.6 : 1);
    float friction = 0.3f;

    if (flying) {
        moveSpeed *= 4;
        friction = 0.15f;
    }
    else {
        if (input.keyDown(GLFW_KEY_SPACE) && isOnGround()) vel.y = JUMP_VEL;
    }

    //Calculate movement vector from camera angle.
    auto& camera = renderer.camera;
    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera.getFront().x, 0, camera.getFront().z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera.getRight().x, 0, camera.getRight().z));

    glm::vec3 mod {0, 0, 0};

    if (input.keyDown(GLFW_KEY_W)) mod += frontFlat;
    if (input.keyDown(GLFW_KEY_S)) mod -= frontFlat;
    if (input.keyDown(GLFW_KEY_D)) mod += rightFlat;
    if (input.keyDown(GLFW_KEY_A)) mod -= rightFlat;

    if (flying) {
        if (input.keyDown(GLFW_KEY_SPACE)) mod.y += 1;
        if (input.keyDown(GLFW_KEY_LEFT_SHIFT)) mod.y -= 1;
    }
    else {
        if (!isOnGround()) vel.y = std::fmax(vel.y - 0.0085, -3);
        else if (vel.y < 0) vel.y = 0;
    }

    if (glm::length(mod) != 0) mod = glm::normalize(mod);
    mod = mod * static_cast<float>(moveSpeed);

    if (!flying) {
        glm::vec3 velFlat = {vel.x, 0, vel.z};
        //Add movement vector with friction.
        velFlat = velFlat * (1.0f-friction) + mod * friction;

        vel.x = velFlat.x;
        vel.z = velFlat.z;
    }
    else {
        //If flying factor in vertical mod values.
        vel = vel * (1.0f-friction) + mod * friction;
    }

    //View movement
    mouseDelta.x *= MOUSE_SENSITIVITY;
    mouseDelta.y *= MOUSE_SENSITIVITY;

    yaw += mouseDelta.x;
    pitch += mouseDelta.y;

    while (yaw > 360.f) yaw -= 360.f;
    while (yaw < 0.f)   yaw += 360.f;

    pitch = std::fmin(std::fmax(pitch, -90), 90);

    moveCollide(isOnGround() ? 0.6 : vel.y <= 0 ? 0.1 : 0);
    updateCamera();
}

void Player::updateCamera() {
    renderer.camera.setYaw(yaw);
    renderer.camera.setPitch(pitch);

    auto type = game.defs.fromId(activeBlock).type;

    glm::vec3 eyesPos = {pos.x, pos.y + EYE_HEIGHT, pos.z};
    renderer.camera.setPos(eyesPos);

    float pitch = std::min(std::max(this->pitch, -89.9f), 89.9f);

    glm::vec3 front = glm::normalize(glm::vec3 {
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))});

    glm::vec3 right = glm::normalize(glm::cross(front, {0, 1, 0}));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    glm::vec3 handPos = eyesPos + front * 0.25f + right * 0.25f + up * (type == ItemDef::Type::CRAFTITEM ? -0.15f : -0.2f);

    handModel.setRotateY(-yaw);
    handModel.setRotateZ(pitch);

    if (type == ItemDef::Type::CRAFTITEM) {
        handItemModel.setRotateX(45);
        handItemModel.setRotateY(110);
        handItemModel.setRotateZ(-25);
    }
    else {
        handItemModel.setRotateX(0);
        handItemModel.setRotateY(0);
        handItemModel.setRotateZ(0);
    }

    handItemModel.setPos(handPos + vel * 0.1f);
    handItemModel.setScale((type == ItemDef::Type::CRAFTITEM ? 0.2f : 0.12f));
}

void Player::findPointedThing(Input &input) {
    glm::ivec3 chunkPos = {};

    std::unique_lock<std::mutex> lock {};
    std::shared_ptr<Chunk> blockChunk = nullptr;

    for (Ray ray(this); ray.getLength() < LOOK_DISTANCE; ray.step(LOOK_PRECISION)) {
        glm::vec3 rayEnd = ray.getEnd();
        glm::ivec3 roundedPos = glm::floor(rayEnd);

        glm::ivec3 currChunkPos = Space::Chunk::world::fromBlock(roundedPos);
        if (currChunkPos != chunkPos || blockChunk == nullptr) {
            chunkPos = currChunkPos;
            blockChunk = world.getChunk(chunkPos);
            if (blockChunk == nullptr) continue;

            lock = blockChunk->aquireLock();
        }

        unsigned int blockID = blockChunk->getBlock(Space::Block::relative::toChunk(roundedPos));
        auto& boxes = game.defs.blockFromId(blockID).sBoxes;

        for (auto& sBox : boxes) {
            auto face = sBox.intersects(rayEnd, roundedPos);

            if (face != Dir::NONE) {
                pointedThing.thing = PointedThing::Thing::BLOCK;
                pointedThing.target.block = { blockID, roundedPos, face };
                return;
            }
        }
    }

    pointedThing.thing = PointedThing::Thing::NOTHING;
    pointedThing.target.nothing = 0;
}

void Player::updateWireframe() {
    if (!gameGui.isVisible()) {
        wireframe.setVisible(false);
    }
    else if (pointedThing.thing == PointedThing::Thing::BLOCK) {
        auto& boxes = game.defs.blockFromId(pointedThing.target.block.blockId).sBoxes;
        float distance = glm::distance(pos, glm::vec3(pointedThing.target.block.pos) + glm::vec3(0.5));

        wireframe.updateMesh(boxes, 0.002f + distance * 0.0014f);
        wireframe.setPos(pointedThing.target.block.pos);
        wireframe.setVisible(true);
    }
    else {
        wireframe.setVisible(false);
    }
}

void Player::breakBlock(Input& input, double delta) {
    if (pointedThing.thing == PointedThing::Thing::BLOCK) {
        if (input.mouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
            if (breakInterval == 0) {
                world.damageBlock(pointedThing.target.block.pos, BLOCK_DAMAGE);
            }
            breakInterval += delta;
            if (breakInterval > BLOCK_INTERVAL) breakInterval = 0;
        }
        else {
            if (breakInterval > 0) breakInterval += delta;
            if (breakInterval > BLOCK_INTERVAL) breakInterval = 0;
        }

        if (input.mousePressed(GLFW_MOUSE_BUTTON_RIGHT) && activeBlock != DefinitionAtlas::AIR) {
            world.localSetBlock(pointedThing.target.block.pos + SelectionBox::faceToOffset(pointedThing.target.block.face), activeBlock);
        }
    }
    else breakInterval = 0;
}

/*
 * Position, Velocity, Yaw, Pitch Getters & Setters
 */

void Player::setPos(glm::vec3 pos) {
    this->pos = pos;
    this->renderer.camera.setPos({pos.x, pos.y + EYE_HEIGHT, pos.z});
}

glm::vec3 Player::getPos() {
    return pos;
}

void Player::setVel(glm::vec3 vel) {
    this->vel = vel;
}

glm::vec3 Player::getVel() {
    return vel;
}

void Player::setYaw(float yaw) {
    this->yaw = yaw;
}

float Player::getYaw() {
    return yaw;
}

void Player::setPitch(float pitch) {
    this->pitch = pitch;
}

float Player::getPitch() {
    return pitch;
}

void Player::setFlying(bool flying) {
    this->flying = flying;
}

bool Player::isFlying() {
    return flying;
}

PointedThing& Player::getPointedThing() {
    return pointedThing;
}

LocalInventory& Player::getInventory() {
    return *refs.getInv("current_player");
}

/*
 * GUI Functions
 */

void Player::setActiveBlock(const std::string& block) {
    activeBlock = game.defs.fromStr(block).index;
    handItemModel.setModel(game.defs.fromId(activeBlock).entityModel);
}

void Player::showMenu(std::shared_ptr<LuaGuiElement> root) {
    gameGui.showMenu(root);
    renderer.window.input.lockMouse(false);
}

void Player::closeMenu() {
    gameGui.closeMenu();
    renderer.window.input.lockMouse(true);
}

bool Player::isInMenu() {
    return gameGui.isInMenu();
}

void Player::setHud(std::shared_ptr<LuaGuiElement> hud) {
    gameGui.setHud(hud);
}

std::shared_ptr<LuaGuiElement> Player::getHud() {
    return gameGui.getHud();
}


void Player::setHudVisible(bool hudVisible) {
    gameGui.setVisible(hudVisible);
}

/*
 * Render functions
 */

void Player::draw(Renderer &renderer) {
    wireframe.draw(renderer);
    handItemModel.draw(renderer);
}

void Player::drawHud(Renderer &renderer) {
    gameGui.drawHud(renderer);
}

void Player::drawMenu(Renderer &renderer) {
    gameGui.drawMenu(renderer);
}

Player::~Player() {
    renderer.window.removeResizeCallback("player");
}