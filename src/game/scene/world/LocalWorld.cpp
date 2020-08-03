//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "LocalPlayer.h"
#include "../../graph/Renderer.h"
#include "../../../net/PacketView.h"
#include "WorldInterpolationStream.h"

LocalWorld::LocalWorld(SubgamePtr game, ServerConnection& conn, Renderer& renderer) :
    World(game),
    renderer(renderer),
    net(conn, *this),
    refs(std::make_shared<LocalInventoryRefs>(game, net)),
    worldGenStream(std::make_shared<WorldInterpolationStream>(55, game)),
    player(std::make_shared<LocalPlayer>(game, *this, DimensionPtr(nullptr), renderer)) {}

void LocalWorld::connect() {
    net.init(Util::bind_this(&(*refs), &LocalInventoryRefs::packetReceived));
    refs->init();
}

bool LocalWorld::updatePlayerDimension() {
    if (defaultDimension.empty()) return false;
    player->setDimension(getDefaultDimension());
    activeDimension = getDefaultDimension().l();
    return true;
}

void LocalWorld::update(double delta) {
    World::update(delta);

    if (*player) player.l()->update(renderer.window.input, delta, renderer.window.input.mouseDelta());
    refs->update(delta, net);
    net.update();

    auto finishedChunks = worldGenStream->update();
    mapBlocksInterpolated = finishedChunks->size() / 64;
    for (const auto &chunk : *finishedChunks) commitChunk(chunk);
}

void LocalWorld::handleWorldPacket(std::unique_ptr<PacketView> p) {
    worldGenStream->queuePacket(std::move(p));
}

void LocalWorld::handlePlayerEntPacket(std::unique_ptr<PacketView> p) {
    if (!player) throw std::runtime_error("Received playerEnt info *before* the player was created.");

    unsigned int id = p->d.read<unsigned int>();
    if (player->getId() == id) return;

    bool found = false;
    for (auto& entity : getActiveDimension().l()->playerEntities) {
        if (entity.getId() == id) {
            entity.interpPos(p->d.read<glm::vec3>());
            entity.interpRotateZ(-p->d.read<float>() + 90);
            entity.interpRotateY(-p->d.read<float>() + 90);

            found = true;
            break;
        }
    }
    if (found) return;

    //TODO: Reimplement player models.
//    auto playerModel = std::make_shared<Model>();
//    playerModel->fromSerialized(static_cast<LocalSubgame&>(game).models.models["zeus:default:player"],
//        { static_cast<LocalSubgame&>(game).textures["zeus:default:player"] });
//    getActiveDimension().playerEntities.emplace_back(p->d.read<glm::vec3>(), id, playerModel);
}

void LocalWorld::commitChunk(std::shared_ptr<Chunk> c) {
    activeDimension->setChunk(std::move(c));
}

DimensionPtr LocalWorld::createDimension(const std::string &identifier) {
    this->dimensions.emplace_back(std::make_shared<LocalDimension>(game, *this, identifier, this->dimensions.size()));
    return dimensions[dimensions.size() - 1];
}

DimensionPtr LocalWorld::getActiveDimension() {
    return activeDimension;
}

void LocalWorld::setActiveDimension(DimensionPtr dim) {
    this->activeDimension = dim.l();
}

ClientNetworkInterpreter& LocalWorld::getNet() {
    return net;
}

PlayerPtr LocalWorld::getPlayer() {
    return player;
}

InventoryRefsPtr LocalWorld::getRefs() {
    return refs;
}

int LocalWorld::renderChunks(Renderer &renderer) {
    return activeDimension->renderChunks(renderer);
}

void LocalWorld::renderEntities(Renderer &renderer) {
    activeDimension->renderEntities(renderer);
    player.l()->draw(renderer);
}

//void LocalWorld::updateBlockDamages(double delta) {
//    auto it = crackEntities.cbegin();
//    while (it != crackEntities.cend()) {
//        bool deleteMe = false;
//
//        auto curr = it++;
//        auto block = curr->second;
//
//        block->time += delta;
//
//        if (block->damage >= block->maxHealth) {
//            //Todo: Lua callback~
//            setBlock(block->getPos(), DefinitionAtlas::AIR);
//            setBlockDamage(block->getPos(), 0);
//            deleteMe = true;
//        }
//
//        if (block->time > 2) {
//            block->update();
//            block->addDamage(-1);
//            block->time = 0;
//        }
//
//        if (block->damage < 0 || block->def.index != getBlock(block->getPos())) {
//            deleteMe = true;
//        }
//
//        if (deleteMe) {
//            delete block;
//            it = crackEntities.erase(curr);
//        }
//        else block->update();
//    }
//}
