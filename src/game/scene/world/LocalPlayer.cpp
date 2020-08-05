//
// Created by aurailus on 28/12/18.
//

#include "LocalPlayer.h"

#include "LocalWorld.h"
#include "../../../util/Ray.h"
#include "../../graph/Renderer.h"
#include "../../entity/Collision.h"
#include "../../../net/Deserializer.h"
#include "../../../def/item/BlockDef.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../net/client/ClientNetworkInterpreter.h"
#include "NetField.h"

LocalPlayer::LocalPlayer(SubgamePtr game, LocalWorld& world, DimensionPtr dim, Renderer &renderer) :
    Player(game, world, dim), DrawableEntity(game, dim), Entity(game, dim),
    renderer(renderer),
    wireframe(game, dim, {1, 1, 1}),
    gameGui(world.getRefs().l(), renderer.window.getSize(), game.l(), renderer) {

    handItemModel.parent = &handModel;

    renderer.window.addResizeCallback("player", [&](glm::ivec2 win) { gameGui.winResized(win); });
}

void LocalPlayer::update(Input &input, double delta, glm::vec2 mouseDelta) {
    gameGui.update(delta);
    handItemModel.setVisible(gameGui.isVisible());

    updatePhysics(input, delta, mouseDelta);

    Collision::moveCollide(game, dim, collision, pos, vel,
        Collision::isOnGround(game, dim, collision, pos, vel) ? 0.6 : vel.y <= 0 ? 0.1 : 0);

    updateCamera();

    findPointedThing(input);
    updateWireframe();

    if (!gameGui.isInMenu()) interact(input, delta);
}

void LocalPlayer::assertField(Packet packet) {
    packet.type = Packet::Type::THIS_PLAYER_INFO;
    static_cast<LocalWorld&>(world).getNet().sendPacket(packet, Packet::Channel::INTERACT);
}

void LocalPlayer::handleAssertion(Deserializer &d) {
    while (!d.atEnd()) {
        switch (d.readE<NetField>()) {
            case NetField::ID: setId(d.read<unsigned int>()); break;
            case NetField::POS: setPos(d.read<glm::vec3>()); break;
            case NetField::VEL: setVel(d.read<glm::vec3>()); break;
            case NetField::LOOK_YAW: setYaw(d.read<float>()); break;
            case NetField::LOOK_PITCH: setPitch(d.read<float>()); break;
            case NetField::FLYING: setFlying(d.read<bool>()); break;

            case NetField::HAND_INV: setHandList(d.read<std::string>()); break;
            case NetField::WIELD_INV: setWieldList(d.read<std::string>()); break;
            case NetField::WIELD_INDEX: setWieldIndex(d.read<unsigned short>()); break;
        }
    }
}

//
// Overridden Setters
//

void LocalPlayer::setPos(glm::vec3 pos, bool assert) {
    Player::setPos(pos, assert);
    this->renderer.camera.setPos(pos + getLookOffset());
}

void LocalPlayer::setLookOffset(glm::vec3 eyeOffset, bool assert) {
    Player::setLookOffset(eyeOffset, assert);
    this->renderer.camera.setPos(pos + getLookOffset());
}


void LocalPlayer::setHandList(const std::string &list, bool assert) {
    Player::setHandList(list, assert);
    world.getRefs().l()->setHandList(list);
    updateWieldAndHandItems();
}

void LocalPlayer::setWieldList(const std::string& list, bool assert) {
    Player::setWieldList(list, assert);
    world.getRefs().l()->setWieldList(list);
    setWieldIndex(wieldIndex);
    updateWieldAndHandItems();
}

void LocalPlayer::setWieldIndex(unsigned short index, bool assert) {
    auto wieldList = world.getRefs().l()->getWieldList();
    wieldIndex = index % std::max((wieldList ? wieldList->getLength() : 1), 1);
    Player::setWieldIndex(wieldIndex, assert);
    updateWieldAndHandItems();
}

void LocalPlayer::setDim(DimensionPtr dim) {
    Player::setDim(dim);
    static_cast<LocalWorld&>(world).setActiveDimension(dim);
}

//
// UI Related
//

bool LocalPlayer::isInMenu() {
    return gameGui.isInMenu();
}

void LocalPlayer::showMenu(std::shared_ptr<LuaGuiElement> root) {
    gameGui.showMenu(root);
    renderer.window.input.lockMouse(false);
}

void LocalPlayer::closeMenu() {
    gameGui.closeMenu();
    renderer.window.input.lockMouse(true);
}

void LocalPlayer::setHud(std::shared_ptr<LuaGuiElement> hud) {
    gameGui.setHud(hud);
}

std::shared_ptr<LuaGuiElement> LocalPlayer::getHud() {
    return gameGui.getHud();
}

void LocalPlayer::setHudVisible(bool hudVisible) {
    gameGui.setVisible(hudVisible);
}

//
// Misc Getters
//

InventoryPtr LocalPlayer::getInventory() {
    return dim->getWorld().getRefs()->getInventory("current_player");
}

Target& LocalPlayer::getTarget() {
    return target;
}

//
// Draw Functions
//

void LocalPlayer::draw(Renderer &renderer) {
    wireframe.draw(renderer);
    handItemModel.draw(renderer);
}

void LocalPlayer::drawHud(Renderer &renderer) {
    gameGui.drawHud(renderer);
}

void LocalPlayer::drawMenu(Renderer &renderer) {
    gameGui.drawMenu(renderer);
}

//
// Private uncategorized.
// TODO: Categorize and optimize.
//

bool LocalPlayer::getKey(Input& input, LocalPlayer::PlayerControl control) {
    if (gameGui.isInMenu()) return false;
    return input.keyDown(
        control == PlayerControl::FORWARD  ? GLFW_KEY_W :
        control == PlayerControl::BACKWARD ? GLFW_KEY_S :
        control == PlayerControl::LEFT     ? GLFW_KEY_A :
        control == PlayerControl::RIGHT    ? GLFW_KEY_D :
        control == PlayerControl::JUMP     ? GLFW_KEY_SPACE :
        control == PlayerControl::MOD1     ? GLFW_KEY_LEFT_SHIFT :
                                             GLFW_KEY_LEFT_CONTROL);
}

void LocalPlayer::updatePhysics(Input &input, double delta, glm::vec2 mouseDelta) {
    static constexpr float JUMP_VEL = 0.14f;
    static constexpr float BASE_MOVE_SPEED = 4.3f;
    static constexpr float MOUSE_SENSITIVITY = 0.1f;

    //Position movement
    bool sprinting = getKey(input, PlayerControl::MOD2);

    double moveSpeed = BASE_MOVE_SPEED * delta * (sprinting ? 1.6 : 1);
    float friction = 0.3f;

    if (flying) {
        moveSpeed *= 4;
        friction = 0.15f;
    }
    else if (getKey(input, PlayerControl::JUMP) &&
        Collision::isOnGround(game, dim, collision, pos, vel)) vel.y = JUMP_VEL;

    //Calculate movement vector from camera angle.
    auto& camera = renderer.camera;
    glm::vec3 frontFlat = glm::normalize(glm::vec3(camera.getFront().x, 0, camera.getFront().z));
    glm::vec3 rightFlat = glm::normalize(glm::vec3(camera.getRight().x, 0, camera.getRight().z));

    glm::vec3 mod {0, 0, 0};

    if (getKey(input, PlayerControl::FORWARD))  mod += frontFlat;
    if (getKey(input, PlayerControl::BACKWARD)) mod -= frontFlat;
    if (getKey(input, PlayerControl::RIGHT))    mod += rightFlat;
    if (getKey(input, PlayerControl::LEFT))     mod -= rightFlat;

    if (flying) {
        if (getKey(input, PlayerControl::JUMP)) mod.y += 1;
        if (getKey(input, PlayerControl::MOD1)) mod.y -= 1;
    }
    else {
        if (!Collision::isOnGround(game, dim, collision, pos, vel)) vel.y = std::fmax(vel.y - 0.0085, -3);
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
}

void LocalPlayer::updateCamera() {
    renderer.camera.setYaw(yaw);
    renderer.camera.setPitch(pitch);

    auto type = game->getDefs().fromId(wieldItem).type;

    glm::vec3 eyesPos = pos + getLookOffset();
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

void LocalPlayer::findPointedThing(Input &input) {
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;

    glm::ivec3 chunkPos = {};

    std::unique_lock<std::mutex> lock {};
    std::shared_ptr<Chunk> blockChunk = nullptr;

    for (Ray ray(*this); ray.getLength() < LOOK_DISTANCE; ray.step(LOOK_PRECISION)) {
        glm::vec3 rayEnd = ray.getEnd();
        glm::ivec3 roundedPos = glm::floor(rayEnd);

        glm::ivec3 currChunkPos = Space::Chunk::world::fromBlock(roundedPos);
        if (currChunkPos != chunkPos || blockChunk == nullptr) {
            chunkPos = currChunkPos;
            blockChunk = dim->getChunk(chunkPos);
            if (blockChunk == nullptr) continue;

            lock = blockChunk->aquireLock();
        }

        unsigned int blockID = blockChunk->getBlock(Space::Block::relative::toChunk(roundedPos));
        auto& boxes = game->getDefs().blockFromId(blockID).sBoxes;

        for (auto& sBox : boxes) {
            auto face = sBox.intersects(rayEnd, roundedPos);

            if (face != EVec::NONE) {
                target = Target(dim, roundedPos, face);
                return;
            }
        }
    }

    target = Target {};
}

void LocalPlayer::updateWireframe() {
    if (!gameGui.isVisible()) {
        wireframe.setVisible(false);
    }
    else if (target.type == Target::Type::BLOCK) {
        auto& boxes = game->getDefs().blockFromId(dim->getBlock(target.pos)).sBoxes;
        float distance = glm::distance(pos, target.pos + glm::vec3(0.5));

        wireframe.updateMesh(boxes, 0.002f + distance * 0.0014f);
        wireframe.setPos(target.pos);
        wireframe.setVisible(true);
    }
    else {
        wireframe.setVisible(false);
    }
}

void LocalPlayer::interact(Input& input, double delta) {
    if (target.type == Target::Type::BLOCK) {
        if (input.mouseDown(GLFW_MOUSE_BUTTON_LEFT) && breakTime == 0) {
            breakInterval = dim->blockHit(target, static_cast<LocalWorld&>(dim->getWorld()).getPlayer());
            breakTime += delta;
        }
        else if (input.mousePressed(GLFW_MOUSE_BUTTON_RIGHT))
            dim->blockPlaceOrInteract(target, static_cast<LocalWorld&>(dim->getWorld()).getPlayer());
    }

    if (breakTime > 0) breakTime += delta;
    if (breakTime > breakInterval) breakTime = 0;
}

void LocalPlayer::updateWieldAndHandItems() {
    auto handList = world.getRefs().l()->getHandList();
    auto wieldList = world.getRefs().l()->getWieldList();

    handItem = handList && handList->getLength() > 0 ? handList->getStack(0).id : 0;
    wieldItem = wieldList && wieldList->getLength() > wieldIndex ? wieldList->getStack(wieldIndex).id : 0;

    auto& model = game->getDefs().fromId(wieldItem <= DefinitionAtlas::AIR ? handItem : wieldItem).entityModel;
    handItemModel.setModel(model);
}

LocalPlayer::~LocalPlayer() {
    renderer.window.removeResizeCallback("player");
}