//
// Created by aurailus on 28/12/18.
//

#include "Player.h"
#include "../../../util/Ray.h"

Player::Player(LocalWorld& world, LocalDefs& defs, Renderer& renderer) :
    Collidable(world, defs, {{-0.3, 0, -0.3}, {0.3, 1.8, 0.3}}),

    defs(defs),
    renderer(renderer),
    wireframe({}, 0.01, {1, 1, 1}),
    gameGui(renderer.window.getSize(), defs, renderer) {}

void Player::update(Input &input, double delta, glm::vec2 mouseDelta) {
    if (activeBlock == -1) activeBlock = defs.defs.blockFromStr("zeus:default:stone").index;

    if (renderer.window.input.isKeyDown(GLFW_KEY_G)) {
        gameGui.list.addStack({defs.defs.craftItemFromStr("zeus:materials:rock").index, 1});
        gameGui.list.addStack({defs.defs.craftItemFromStr("zeus:materials:stick").index, 2});
    }
    gameGui.update(delta);

    renderer.window.addResizeCallback("player", [&](glm::ivec2 win) {
        gameGui.winResized(win);
    });

    if (input.isKeyPressed(GLFW_KEY_F)) flying = !flying; //TODO: Move to Lua Bind

    moveAndLook(input, delta, mouseDelta);
    findPointedThing(input);
    updateWireframe();
    breakBlock(input, delta);
}

void Player::moveAndLook(Input &input, double delta, glm::vec2 mouseDelta) {
    //Position movement
    bool sprinting = input.isKeyDown(GLFW_KEY_LEFT_CONTROL);

    double moveSpeed = BASE_MOVE_SPEED * delta * (sprinting ? 1.6 : 1);
    float friction = 0.3f;

    if (flying) {
        moveSpeed *= 4;
        friction = 0.15f;
    }
    else {
        if (input.isKeyDown(GLFW_KEY_SPACE) && isOnGround()) vel.y = JUMP_VEL;
    }

    //Calculate movement vector from camera angle.
    auto& camera = renderer.camera;
    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera.getFront().x, 0, camera.getFront().z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera.getRight().x, 0, camera.getRight().z));

    glm::vec3 mod {0, 0, 0};

    if (input.isKeyDown(GLFW_KEY_W)) mod += frontFlat;
    if (input.isKeyDown(GLFW_KEY_S)) mod -= frontFlat;
    if (input.isKeyDown(GLFW_KEY_D)) mod += rightFlat;
    if (input.isKeyDown(GLFW_KEY_A)) mod -= rightFlat;

    if (flying) {
        if (input.isKeyDown(GLFW_KEY_SPACE)) mod.y += 1;
        if (input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) mod.y -= 1;
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
    renderer.camera.setPos({pos.x, pos.y + EYE_HEIGHT, pos.z});
}

void Player::findPointedThing(Input &input) {
    glm::vec3 chunkPos = {};
    sptr<BlockChunk> blockChunk = nullptr;

    for (Ray ray(this); ray.getLength() < LOOK_DISTANCE; ray.step(LOOK_PRECISION)) {
        glm::vec3 rayEnd = ray.getEnd();
        glm::vec3 roundedPos = glm::floor(rayEnd);

        auto currChunkPos = Space::Chunk::world::fromBlock(roundedPos);
        if (currChunkPos != chunkPos || blockChunk == nullptr) {
            chunkPos = currChunkPos;
            blockChunk = world.getChunk(chunkPos);
        }

        unsigned int blockID = 0;
        if (blockChunk != nullptr) {
            blockID = blockChunk->getBlock(Space::Block::relative::toChunk(roundedPos));
        }

        auto& boxes = defs.defs.blockFromId(blockID).sBoxes;

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
        auto& boxes = defs.defs.blockFromId(pointedThing.target.block.blockId).sBoxes;
        float distance = glm::distance(pos, pointedThing.target.block.pos + glm::vec3(0.5));

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
        if (input.isMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
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

        if (input.isMousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
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

/*
 * Pointed Thing
 */

PointedThing& Player::getPointedThing() {
    return pointedThing;
}

/*
 * GUI Functions
 */

void Player::setActiveBlock(const std::string& block) {
    activeBlock = defs.defs.blockFromStr(block).index;
}

void Player::setMenu(const std::string& menu, const std::map<std::string, GuiBuilder::ComponentCallbacks>& callbacks) {
    gameGui.setMenu(menu, callbacks);
    renderer.window.lockMouse(false);
}

void Player::closeMenu() {
    gameGui.closeMenu();
    renderer.window.lockMouse(true);
}

void Player::setGuiVisible(bool hudVisible) {
    gameGui.setVisible(hudVisible);
}

std::string Player::getMenuState() {
    return gameGui.getMenuState();
}

/*
 * Render functions
 */

void Player::draw(Renderer &renderer) {
    wireframe.draw(renderer);
}

void Player::drawGUI(Renderer &renderer) {
    gameGui.draw(renderer);
}

void Player::drawViginette(Renderer &renderer) {
    gameGui.drawViginette(renderer);
}

Player::~Player() {
    renderer.window.removeResizeCallback("player");
}
