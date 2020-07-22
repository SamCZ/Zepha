//
// Created by aurailus on 28/12/18.
//

#include "Player.h"

#include "LocalWorld.h"
#include "../../../util/Ray.h"
#include "../../graph/Renderer.h"
#include "../../../def/ItemDef.h"
#include "../../../net/Serializer.h"
#include "../../../net/Deserializer.h"
#include "../../../def/item/BlockDef.h"
#include "../../../world/chunk/Chunk.h"
#include "../../inventory/LocalInventory.h"
#include "../../inventory/LocalInventoryList.h"
#include "../../../net/client/NetPlayerField.h"

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

    if (!gameGui.isInMenu()) interact(input, delta);
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

    auto type = game.defs.fromId(wieldItem).type;

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

            if (face != EVec::NONE) {
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

void Player::interact(Input& input, double delta) {
    if (pointedThing.thing == PointedThing::Thing::BLOCK) {
        if (input.mouseDown(GLFW_MOUSE_BUTTON_LEFT) && breakTime == 0) {
            breakInterval = world.blockHit(pointedThing);
            breakTime += delta;
        }
        else if (input.mousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            auto& target = game.defs.blockFromId(world.getBlock(pointedThing.target.block.pos));

            if (target.hasInteraction()) world.blockInteract(pointedThing);
            else if (wieldItem > DefinitionAtlas::AIR && game.defs.fromId(wieldItem).type == ItemDef::Type::BLOCK) {
                world.blockPlace(pointedThing.target.block.pos + SelectionBox::faceToOffset(pointedThing.target.block.face), wieldItem);
            }
            else {
                //TODO: Item interactions.
//                world.itemUse(pointedThing, activeItem);
            }
        }
    }

    if (breakTime > 0) breakTime += delta;
    if (breakTime > breakInterval) breakTime = 0;
}

//
// Getters and Setters
//

glm::vec3 Player::getPos() {
    return pos;
}

void Player::setPos(glm::vec3 pos, bool assert) {
    this->pos = pos;
    this->renderer.camera.setPos({pos.x, pos.y + EYE_HEIGHT, pos.z});
    if (assert) assertField(NetPlayerField::POSITION, pos);
}

glm::vec3 Player::getVel() {
    return vel;
}

void Player::setVel(glm::vec3 vel, bool assert) {
    this->vel = vel;
    if (assert) assertField(NetPlayerField::VELOCITY, vel);
}

float Player::getYaw() {
    return yaw;
}

void Player::setYaw(float yaw, bool assert) {
    this->yaw = yaw;
    if (assert) assertField(NetPlayerField::YAW, yaw);
}

float Player::getPitch() {
    return pitch;
}

void Player::setPitch(float pitch, bool assert) {
    this->pitch = pitch;
    if (assert) assertField(NetPlayerField::PITCH, pitch);
}

bool Player::isFlying() {
    return flying;
}

void Player::setFlying(bool flying, bool assert) {
    this->flying = flying;
    if (assert) assertField(NetPlayerField::FLYING, flying);
}

PointedThing& Player::getPointedThing() {
    return pointedThing;
}

LocalInventory& Player::getInventory() {
    return *refs.getInv("current_player");
}

std::shared_ptr<LocalInventoryList> Player::getHandList() {
    return refs.getHandList();
}

void Player::setHandList(const std::string& list) {
    refs.setHandList(list);
    updateWieldAndHandItems();
}

std::shared_ptr<LocalInventoryList> Player::getWieldList() {
    return refs.getWieldList();
}

void Player::setWieldList(const std::string& list, bool assert) {
    refs.setWieldList(list);
    setWieldIndex(wieldIndex);
    updateWieldAndHandItems();
    if (assert) assertField(NetPlayerField::WIELD_INV, list);
}

unsigned short Player::getWieldIndex() {
    return wieldIndex;
}

void Player::setWieldIndex(unsigned short index, bool assert) {
    auto wieldList = refs.getWieldList();
    wieldIndex = index % std::max((wieldList ? wieldList->getLength() : 1), 1);
    updateWieldAndHandItems();
    if (assert) assertField(NetPlayerField::WIELD_INV, static_cast<unsigned short>(index));
}

/*
 * GUI Functions
 */

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

void Player::updateWieldAndHandItems() {
    auto handList = refs.getHandList();
    auto wieldList = refs.getWieldList();

    handItem = handList && handList->getLength() > 0 ? handList->getStack(0).id : 0;
    wieldItem = wieldList && wieldList->getLength() > wieldIndex ? wieldList->getStack(wieldIndex).id : 0;

    auto& model = game.defs.fromId(wieldItem <= DefinitionAtlas::AIR ? handItem : wieldItem).entityModel;
    handItemModel.setModel(model);
}

template <typename T>
void Player::assertField(NetPlayerField field, T data) {
    std::cout << "attempt to assert field" << std::endl;
//    Serializer().append(static_cast<unsigned int>(field)).append<T>(data)
//            .packet(PacketType::THIS_PLAYER_INFO).sendTo(peer, PacketChannel::PLAYER);
}

void Player::handleAssertion(Deserializer &d) {
    while (!d.atEnd()) {
        switch (static_cast<NetPlayerField>(d.read<unsigned int>())) {
            case NetPlayerField::ID: id = d.read<unsigned int>(); break;
            case NetPlayerField::POSITION: setPos(d.read<glm::vec3>()); break;
            case NetPlayerField::VELOCITY: setVel(d.read<glm::vec3>()); break;
            case NetPlayerField::PITCH: setPitch(d.read<float>()); break;
            case NetPlayerField::YAW: setYaw(d.read<float>()); break;

            case NetPlayerField::FLYING: setFlying(d.read<bool>()); break;

            case NetPlayerField::HAND_INV: setHandList(d.read<std::string>()); break;
            case NetPlayerField::WIELD_INV: setWieldList(d.read<std::string>()); break;
            case NetPlayerField::WIELD_INDEX: setWieldIndex(d.read<unsigned short>()); break;
        }
    }
}

Player::~Player() {
    renderer.window.removeResizeCallback("player");
}