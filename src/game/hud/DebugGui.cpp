//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"


#include "../../def/ClientGame.h"
#include "../scene/world/Player.h"
#include "../../def/texture/Font.h"
#include "../../def/gen/BiomeDef.h"
#include "../../def/item/BlockDef.h"
#include "components/basic/GuiText.h"
#include "../scene/world/LocalWorld.h"
#include "components/compound/GuiLabelledGraph.h"

DebugGui::DebugGui(glm::vec2 bufferSize, ClientGame& defs) :
    displayMode(0) {

    auto fpsHistogramRef = defs.textures["histogram"];
    auto genericHistogramRef = defs.textures["histogram_white"];
    auto fontRef = defs.textures["font"];

    Font f(defs.textures, fontRef);

    auto crosshairText = std::make_shared<GuiText>("crosshairText");
    crosshairText->create({2, 2}, {}, {0.2, 0.2, 0.2, 0.5}, {1, 1, 1, 1}, f);
    add(crosshairText);

    auto dataText = std::make_shared<GuiText>("dataText");
    dataText->create({2, 2}, {}, {0.2, 0.2, 0.2, 0.5}, {1, 1, 1, 1}, f);
    add(dataText);

    auto interpGraph = std::make_shared<GuiLabelledGraph>("interpGraph");
    interpGraph->create({244, 64}, {}, "Interp", 120, 256, genericHistogramRef, f);
    add(interpGraph);

    auto meshGraph = std::make_shared<GuiLabelledGraph>("meshGraph");
    meshGraph->create({244, 64}, {}, "Mesh", 120, 32, genericHistogramRef, f);
    add(meshGraph);

    auto genGraph = std::make_shared<GuiLabelledGraph>("genGraph");
    genGraph->create({244, 64}, {}, "Gen", 120, 16, genericHistogramRef, f);
    add(genGraph);

    auto packetGraph = std::make_shared<GuiLabelledGraph>("packetGraph");
    packetGraph->create({244, 64}, {}, "Packets", 120, 512, genericHistogramRef, f);
    add(packetGraph);

    auto fpsGraph = std::make_shared<GuiLabelledGraph>("fpsGraph");
    fpsGraph->create({244, 64}, {}, "FPS", 120, 60, fpsHistogramRef, f);
    add(fpsGraph);

    auto drawsGraph = std::make_shared<GuiLabelledGraph>("drawsGraph");
    drawsGraph->create({244, 64}, {}, "Draw Calls", 120, 0, genericHistogramRef, f);
    add(drawsGraph);

    auto gpuGraph = std::make_shared<GuiLabelledGraph>("gpuGraph");
    gpuGraph->create({244, 64}, {}, "GPU", 120, 1, genericHistogramRef, f);
    add(gpuGraph);

    positionElements(bufferSize);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth  = static_cast<int>(bufferSize.x);
    auto bufferHeight = static_cast<int>(bufferSize.y);

    get<GuiText>("crosshairText")->setPos({bufferWidth / 2 + 22, bufferHeight / 2 - 7});
    get<GuiText>("dataText")->setPos({10, 10});

    get<GuiLabelledGraph>("genGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 160});
    get<GuiLabelledGraph>("packetGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 240});
    get<GuiLabelledGraph>("meshGraph")->setPos({bufferWidth - 254, bufferHeight - 70 - 80});
    get<GuiLabelledGraph>("interpGraph")->setPos({bufferWidth - 254, bufferHeight - 70});

    get<GuiLabelledGraph>("fpsGraph")->setPos({bufferWidth - 254, 10});
    get<GuiLabelledGraph>("drawsGraph")->setPos({bufferWidth - 254, 90});
    get<GuiLabelledGraph>("gpuGraph")->setPos({bufferWidth - 254, 90 + 80});
}

void DebugGui::update(Player& player, LocalWorld& world, ClientGame& defs, double fps, int /*chunks*/, int drawCalls, int ssGen, int ssPack) {

    { //Top Right Graphs
        get<GuiLabelledGraph>("fpsGraph")->pushValue(static_cast<float>(fps));
        get<GuiLabelledGraph>("drawsGraph")->pushValue(drawCalls);

        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

        get<GuiLabelledGraph>("gpuGraph")->pushValue(static_cast<int>(std::round(
                (videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0))
                                                     / 100.0f);
    }

    { //Bottom Right Graphs
        get<GuiLabelledGraph>("meshGraph")->pushValue(world.lastMeshUpdates);
        get<GuiLabelledGraph>("interpGraph")->pushValue(world.mapBlocksInterpolated);
        get<GuiLabelledGraph>("genGraph")->pushValue(static_cast<float>(ssGen));
        get<GuiLabelledGraph>("packetGraph")->pushValue(static_cast<float>(ssPack));
    }

    { //Top-left Data
        unsigned int biomeID = world.getBiome(glm::floor(player.getPos()));
        std::string biome = defs.biomes.biomeFromId(biomeID).identifier;

        glm::vec3 playerPos = glm::floor(player.getPos());
        glm::vec3 chunkPos = Space::Chunk::world::fromBlock(playerPos);
        glm::vec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
        glm::vec3 regionPos = Space::Region::world::fromChunk(chunkPos);

        //Block Coordinates offset from respective container
        glm::vec3 posOffsetFromChunk  = Space::Block::relative::toChunk(playerPos);
        glm::vec3 posOffsetFromBlock  = Space::Block::relative::toMapBlock(playerPos);
        glm::vec3 posOffsetFromRegion = Space::Block::relative::toRegion(playerPos);

        std::ostringstream str;

        using namespace Util;

        str << "Player: " << vecToString(playerPos);
        str << " (" << floatVecToString(player.getPos()) << ")" << std::endl << std::endl;

        str << "Chunk: " << vecToString(posOffsetFromChunk) << " [" << vecToString(chunkPos) << "]" << std::endl;
        str << "MapBlock: " << vecToString(posOffsetFromBlock) << " [" << vecToString(mapBlockPos) << "]" << std::endl;
        str << "Region: " << vecToString(posOffsetFromRegion) << " [" << vecToString(regionPos) << "]" << std::endl << std::endl;

        str << "Vel: " << floatVecToString(player.getVel()) << std::endl;
        str << "Yaw: " << floatToString(player.getYaw()) << ", ";
        str << "Pitch: " << floatToString(player.getPitch()) << std::endl << std::endl;

        str << "Biome: " << biome << std::endl << std::endl;

        str << "Texture Slots: " << defs.textures.textureSlotsUsed << " / " << defs.textures.maxTextureSlots
            << " (" << round(defs.textures.textureSlotsUsed / static_cast<float>(defs.textures.maxTextureSlots) * 100) << "%)" << std::endl << std::endl;

        PointedThing thing = player.getPointedThing();
        if (thing.thing == PointedThing::Thing::BLOCK) {
            EVec faceDir = thing.target.block.face;

            std::string face =
                faceDir == EVec::TOP    ?  "TOP"   :
                faceDir == EVec::BOTTOM ? "BOTTOM" :
                faceDir == EVec::LEFT   ? "LEFT"   :
                faceDir == EVec::RIGHT  ? "RIGHT"  :
                faceDir == EVec::FRONT  ? "FRONT"  :
                faceDir == EVec::BACK   ? "BACK"   :
                                          "NONE"   ;

            str << "Pointing At: " << defs.defs.blockFromId(thing.target.block.blockId).identifier << std::endl;
            str << "Pointed Position: " << vecToString(thing.target.block.pos) << std::endl;
            str << "Pointed Face: " << face << std::endl;
        }
        else {
            str << "No Target";
        }

        get<GuiText>("dataText")->setText(str.str());
    }

    { //Crosshair Text
        PointedThing thing = player.getPointedThing();

        std::ostringstream crossText;
        if (thing.thing == PointedThing::Thing::BLOCK) {
            crossText << defs.defs.blockFromId(thing.target.block.blockId).name
                      << " (" << defs.defs.blockFromId(thing.target.block.blockId).identifier << ")" << std::endl;
        }
        get<GuiText>("crosshairText")->setText(crossText.str());
    }
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    setVisible(displayMode == 0);
    get<GuiLabelledGraph>("fpsGraph")->setVisible(displayMode != 1);
}
