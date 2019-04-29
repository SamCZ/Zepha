//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

DebugGui::DebugGui(glm::vec2 bufferSize, Texture* tex) {
    displayMode = 0;

    fontTexture         = new Texture("../res/tex/gui/font.png");
    colorHistTexture    = new Texture("../res/tex/gui/histogram.png");
    whiteHistTexture    = new Texture("../res/tex/gui/histogram_white.png");

    atlasTex = new TextureRect(tex);
    atlasTex->setScale({256, 256, 1});

    crosshairText = new TextEntity(fontTexture, true);

    dataText = new TextEntity(fontTexture, true);

    chunkUpdatesGraph = new StatGraph("Interp",     120, 64, whiteHistTexture, fontTexture);
    meshUpdatesGraph  = new StatGraph("Mesh",       120, 64, whiteHistTexture, fontTexture);
    serverGenGraph    = new StatGraph("Gen",        120, 64, whiteHistTexture, fontTexture);
    serverPacketGraph = new StatGraph("Packets",    120, 64, whiteHistTexture, fontTexture);

    fpsGraph          = new StatGraph("FPS",        244, 64, 120, 60, colorHistTexture, fontTexture);
    drawCallsGraph    = new StatGraph("Draw Calls", 244, 64, 120, 0,  whiteHistTexture, fontTexture);
    vRamGraph         = new StatGraph("VRam",       244, 64, 120, 1,  whiteHistTexture, fontTexture);

    positionElements(bufferSize);

    addDrawable(dataText);
    addDrawable(crosshairText);

    addDrawable(chunkUpdatesGraph);
    addDrawable(meshUpdatesGraph);
    addDrawable(serverGenGraph);
    addDrawable(serverPacketGraph);
    addDrawable(fpsGraph);
    addDrawable(drawCallsGraph);
    addDrawable(vRamGraph);

    addDrawable(atlasTex);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth  = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    crosshairText->setPos({bufferWidth / 2 + 22, bufferHeight / 2 - 7, 0});
    atlasTex->setPos({8, 350, 0});

    dataText->setPos(glm::vec3(10, 10, 0));

    serverGenGraph    -> setPosition({bufferWidth - 254, bufferHeight - 70 - 160});
    serverPacketGraph -> setPosition({bufferWidth - 254, bufferHeight - 70 - 240});
    meshUpdatesGraph  -> setPosition({bufferWidth - 254, bufferHeight - 70 - 80 });
    chunkUpdatesGraph -> setPosition({bufferWidth - 254, bufferHeight - 70      });
    fpsGraph          -> setPosition({10,                bufferHeight - 70      });
    drawCallsGraph    -> setPosition({10,                bufferHeight - 70 - 80 });
    vRamGraph         -> setPosition({bufferWidth - 254, 10                     });
}

void DebugGui::update(Player& player, LocalWorld& world, GameDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {

    { //VRam Usage Graph (Top Right)
        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

        vRamGraph->update(static_cast<int>(std::round(
                (videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0))
                / 100.0f);
    }

    { //Bottom Left Graphs
        fpsGraph->update(static_cast<float>(fps));
        drawCallsGraph->update(drawCalls);
    }

    { //Bottom Right Graphs
        meshUpdatesGraph->update(world.lastMeshUpdates);
        chunkUpdatesGraph->update(world.lastGenUpdates);

        serverGenGraph->update(static_cast<float>(ssGen));
        serverPacketGraph->update(static_cast<float>(ssPack));
    }

    { //Top-left Data
        glm::vec3 footPos = TransPos::roundPos(*player.getPos()) + glm::vec3(0, -2, 0);

        int blockID = world.getBlock(footPos);
        std::string on = (blockID > 0) ? defs.blocks().getBlock(blockID).getIdentifier() : "invalid";

        glm::vec3 playerPos = TransPos::roundPos(*player.getPos());

        glm::vec3 chunkPos = TransPos::chunkFromVec(playerPos);

        //Block Coordinates offset from respective container
        glm::vec3 posOffsetFromChunk = TransPos::chunkLocalFromVec(playerPos);
        glm::vec3 posOffsetFromBlock = TransPos::mapBlockLocalFromVec(playerPos);
        glm::vec3 posOffsetFromRegion = TransPos::regionLocalFromVec(playerPos);

        glm::vec3 chunkCoordinate = TransPos::Dimension::chunkOffsetFromMapBlock(chunkPos);
        glm::vec3 mapBlockCoordinate = TransPos::Dimension::mapBlockOffsetFromRegion(chunkPos);
        glm::vec3 regionCoordinate = TransPos::Dimension::regionFromVec(chunkPos);

        std::ostringstream str;

        using namespace Util;

        str << "Player: " << vecToString(playerPos);
        str << " (" << floatVecToString(*player.getPos()) << ")" << std::endl << std::endl;

        str << "Chunk: " << vecToString(posOffsetFromChunk) << " [" << vecToString(chunkPos) << "]" << std::endl;
        str << "MapBlock: " << vecToString(posOffsetFromBlock) << std::endl;
        str << "Region: " << vecToString(posOffsetFromRegion) << std::endl << std::endl;

        str << "Ch: " << vecToString(chunkCoordinate) << ", ";
        str << "Mb: " << vecToString(mapBlockCoordinate) << ", ";
        str << "Rg: " << vecToString(regionCoordinate) << std::endl;

        str << "Vel: " << floatVecToString(*player.getVel()) << std::endl;
        str << "Yaw: " << floatToString(player.getYaw()) << ", ";
        str << "Pitch: " << floatToString(player.getPitch()) << std::endl << std::endl;

        str << "Standing On: " << on << std::endl << std::endl;

        str << "Pointing: " << (player.pointingAtBlock ? "yes" : "no") << ", ";
        str << "PointedBlock: " << vecToString(player.pointedBlock) << std::endl;
        str << "BreakState: " << floatToString(player.digPercentage);

        dataText->set(str.str());
    }

    { //Crosshair Text
        std::string look;
        auto block = player.getPointedBlock();
        if (block != nullptr) {
            look = defs.blocks().getBlock(world.getBlock(*block)).getIdentifier();
        }
        else look = "invalid";

        crosshairText->set(look);
    }
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    for (auto elem : children) {
        elem->setVisible(displayMode == 0);
    }

    fpsGraph->setVisible(displayMode != 1);
}