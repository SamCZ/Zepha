//
// Created by aurailus on 14/12/18.
//

#pragma once

#include "World.h"

#include "../../../world/LocalDimension.h"
#include "../../../net/client/ClientNetworkInterpreter.h"

class Window;
class Renderer;
class LocalPlayer;
class LocalSubgame;
class LocalInventoryRefs;
class WorldInterpolationStream;

class LocalWorld : public World {
public:
    LocalWorld(LocalSubgame& game, ServerConnection& conn, Renderer& window);

    void connect();
    bool initPlayer();
    void update(double delta) override;

    void handleWorldPacket(std::unique_ptr<PacketView> p);
    void handlePlayerEntPacket(std::unique_ptr<PacketView> p);
    void commitChunk(std::shared_ptr<Chunk> chunk);

    virtual LocalDimension& createDimension(const std::string& identifier) override;

    virtual LocalDimension& getDefaultDimension() override;

    virtual LocalDimension& getDimension(unsigned int index) override;
    virtual LocalDimension& getDimension(const std::string& identifier) override;

    std::shared_ptr<LocalDimension> getDefaultDimensionPtr();
    std::shared_ptr<LocalDimension> getDimensionPtr(const std::string& identifier);

    ClientNetworkInterpreter& getNet();
    LocalDimension& getActiveDimension();
    std::shared_ptr<LocalPlayer> getPlayer();
    std::shared_ptr<LocalInventoryRefs> getRefs();

    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);

    int mapBlocksInterpolated = 0;
    int lastMeshUpdates = 0;
private:
    Renderer& renderer;

    ClientNetworkInterpreter net;
    std::shared_ptr<LocalInventoryRefs> refs;
    std::shared_ptr<LocalPlayer> player = nullptr;

    std::shared_ptr<LocalDimension> activeDimension = nullptr;

    std::shared_ptr<WorldInterpolationStream> worldGenStream = nullptr;
};
