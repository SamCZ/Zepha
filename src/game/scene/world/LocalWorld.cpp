//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "LocalPlayer.h"
#include "../../graph/Renderer.h"
#include "../../../net/PacketView.h"
#include "WorldInterpolationStream.h"

LocalWorld::LocalWorld(LocalSubgame& game, ServerConnection& conn, Renderer& renderer) :
    World(game),
    renderer(renderer),
    net(conn, game, *this),
    refs(std::make_shared<LocalInventoryRefs>(game, net)),
    worldGenStream(std::make_shared<WorldInterpolationStream>(55, game)) {}

void LocalWorld::connect() {
    net.init(Util::bind_this(&(*refs), &LocalInventoryRefs::packetReceived));
    refs->init();
}

bool LocalWorld::initPlayer() {
    if (defaultDimension.empty()) return false;
    player = std::make_shared<LocalPlayer>(static_cast<LocalSubgame&>(game), getDefaultDimension(), renderer);
    activeDimension = getDefaultDimensionPtr();
    return true;
}

void LocalWorld::update(double delta) {
    World::update(delta);

    if (player) player->update(renderer.window.input, delta, renderer.window.input.mouseDelta());
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
    for (auto& entity : getActiveDimension().playerEntities) {
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

LocalDimension& LocalWorld::createDimension(const std::string &identifier) {
    this->dimensions.emplace_back(std::make_shared<LocalDimension>(static_cast<LocalSubgame&>(game), *this, identifier, this->dimensions.size()));
    return static_cast<LocalDimension&>(*dimensions[dimensions.size() - 1]);
}

LocalDimension& LocalWorld::getDefaultDimension() {
    return static_cast<LocalDimension&>(World::getDefaultDimension());
}

LocalDimension& LocalWorld::getDimension(unsigned int index) {
    return static_cast<LocalDimension&>(*dimensions[index]);
}

LocalDimension& LocalWorld::getDimension(const std::string &identifier) {
    for (auto& dimension : dimensions)
        if (dimension->getIdentifier() == identifier)
            return static_cast<LocalDimension&>(*dimension);
    throw std::runtime_error("No dimension named " + identifier + " found.");
}

std::shared_ptr<LocalDimension> LocalWorld::getDefaultDimensionPtr() {
    for (auto& dimension : dimensions)
        if (dimension->getIdentifier() == defaultDimension)
            return std::static_pointer_cast<LocalDimension>(dimension);
    throw std::runtime_error("No default dimension set.");
}

std::shared_ptr<LocalDimension> LocalWorld::getDimensionPtr(const std::string &identifier) {
    for (auto& dimension : dimensions)
        if (dimension->getIdentifier() == identifier)
            return std::static_pointer_cast<LocalDimension>(dimension);
    throw std::runtime_error("No dimension named " + identifier + " found.");
}

ClientNetworkInterpreter& LocalWorld::getNet() {
    return net;
}

LocalDimension& LocalWorld::getActiveDimension() {
    return *activeDimension;
}

std::shared_ptr<LocalPlayer> LocalWorld::getPlayer() {
    return player;
}

std::shared_ptr<LocalInventoryRefs> LocalWorld::getRefs() {
    return refs;
}

int LocalWorld::renderChunks(Renderer &renderer) {
    return activeDimension->renderChunks(renderer);
}

void LocalWorld::renderEntities(Renderer &renderer) {
    activeDimension->renderEntities(renderer);
    player->draw(renderer);
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
