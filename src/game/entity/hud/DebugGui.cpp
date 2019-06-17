//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

DebugGui::DebugGui(glm::vec2 bufferSize, Texture* tex) :
    displayMode(0),

    fontTexture("../res/tex/gui/font.png"),
    coloredGraphTexture("../res/tex/gui/histogram.png"),
    monochromeGraphTexture("../res/tex/gui/histogram_white.png"),

//    atlasTex(tex),
    crosshairText(&fontTexture, true),
    dataText(&fontTexture, true),

    chunkUpdatesGraph("Interp",     120, 64, &monochromeGraphTexture, &fontTexture),
    meshUpdatesGraph ("Mesh",       120, 64, &monochromeGraphTexture, &fontTexture),
    serverGenGraph   ("Gen",        120, 64, &monochromeGraphTexture, &fontTexture),
    serverPacketGraph("Packets",    120, 64, &monochromeGraphTexture, &fontTexture),

    fpsGraph         ("FPS",        244, 64, 120, 60, &coloredGraphTexture   , &fontTexture),
    drawCallsGraph   ("Draw Calls", 244, 64, 120, 0,  &monochromeGraphTexture, &fontTexture),
    vRamGraph        ("VRam",       244, 64, 120, 1,  &monochromeGraphTexture, &fontTexture) {

//    atlasTex.setScale({512, 512, 1});

    positionElements(bufferSize);

    children.push_back(&dataText);
    children.push_back(&crosshairText);

    children.push_back(&chunkUpdatesGraph);
    children.push_back(&meshUpdatesGraph);
    children.push_back(&serverGenGraph);
    children.push_back(&serverPacketGraph);
    children.push_back(&fpsGraph);
    children.push_back(&drawCallsGraph);
    children.push_back(&vRamGraph);

//    children.push_back(&atlasTex);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth  = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    crosshairText.setPos({bufferWidth / 2 + 22, bufferHeight / 2 - 7, 0});
//    atlasTex.setPos({8, 350, 0});

    dataText.setPos(glm::vec3(10, 10, 0));

    serverGenGraph   .setPosition({bufferWidth - 254, bufferHeight - 70 - 160});
    serverPacketGraph.setPosition({bufferWidth - 254, bufferHeight - 70 - 240});
    meshUpdatesGraph .setPosition({bufferWidth - 254, bufferHeight - 70 - 80 });
    chunkUpdatesGraph.setPosition({bufferWidth - 254, bufferHeight - 70      });
    fpsGraph         .setPosition({10,                bufferHeight - 70      });
    drawCallsGraph   .setPosition({10,                bufferHeight - 70 - 80 });
    vRamGraph        .setPosition({bufferWidth - 254, 10                     });
}

void DebugGui::update(Player& player, LocalWorld& world, LocalDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {

    { //VRam Usage Graph (Top Right)
        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

        vRamGraph.update(static_cast<int>(std::round(
                (videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0))
                / 100.0f);
    }

    { //Bottom Left Graphs
        fpsGraph.update(static_cast<float>(fps));
        drawCallsGraph.update(drawCalls);
    }

    { //Bottom Right Graphs
        meshUpdatesGraph.update(world.lastMeshUpdates);
        chunkUpdatesGraph.update(world.lastGenUpdates);

        serverGenGraph.update(static_cast<float>(ssGen));
        serverPacketGraph.update(static_cast<float>(ssPack));
    }

    { //Top-left Data
        glm::vec3 footPos = TransPos::roundPos(player.getPos()) + glm::vec3(0, -2, 0);

        int blockID = world.getBlock(footPos);
        std::string on = (blockID > 0) ? defs.blocks().fromIndex(blockID).getIdentifier() : "invalid";

        glm::vec3 playerPos = TransPos::roundPos(player.getPos());

        glm::vec3 chunkPos = TransPos::chunkFromVec(playerPos);

        //Block Coordinates offset from respective container
        glm::vec3 posOffsetFromChunk = TransPos::chunkLocalFromVec(playerPos);
        glm::vec3 posOffsetFromBlock = TransPos::mapBlockLocalFromVec(playerPos);
        glm::vec3 posOffsetFromRegion = TransPos::regionLocalFromVec(playerPos);

        glm::vec3 chunkCoordinate = TransPos::Dimension::chunkOffsetFromMapBlock(chunkPos);
        glm::vec3 mapBlockCoordinate = TransPos::Dimension::mapBlockOffsetFromRegion(chunkPos);
        glm::vec3 regionCoordinate = TransPos::Dimension::regionFromVec(chunkPos);

        auto thing = player.getPointedThing();
        std::string face = (thing.face == SelectionBox::TOP)    ? "TOP" :
                           (thing.face == SelectionBox::BOTTOM) ? "BOTTOM" :
                           (thing.face == SelectionBox::LEFT)   ? "LEFT" :
                           (thing.face == SelectionBox::RIGHT)  ? "RIGHT" :
                           (thing.face == SelectionBox::FRONT)  ? "FRONT" :
                           (thing.face == SelectionBox::BACK)   ? "BACK" :
                           "NONE";

        std::ostringstream str;

        using namespace Util;

        str << "Player: " << vecToString(playerPos);
        str << " (" << floatVecToString(player.getPos()) << ")" << std::endl << std::endl;

        str << "Chunk: " << vecToString(posOffsetFromChunk) << " [" << vecToString(chunkPos) << "]" << std::endl;
        str << "MapBlock: " << vecToString(posOffsetFromBlock) << std::endl;
        str << "Region: " << vecToString(posOffsetFromRegion) << std::endl << std::endl;

        str << "Ch: " << vecToString(chunkCoordinate) << ", ";
        str << "Mb: " << vecToString(mapBlockCoordinate) << ", ";
        str << "Rg: " << vecToString(regionCoordinate) << std::endl;

        str << "Vel: " << floatVecToString(player.getVel()) << std::endl;
        str << "Yaw: " << floatToString(player.getYaw()) << ", ";
        str << "Pitch: " << floatToString(player.getPitch()) << std::endl << std::endl;

        str << "Standing On: " << on << std::endl << std::endl;

        if (thing.blockDef != nullptr) str << "Pointing At: " << thing.blockDef->getIdentifier() << ", " << std::endl;
        str << "Pointed Position: " << vecToString(thing.pos) << std::endl;
        str << "Pointed Face: " << face << std::endl;

        dataText.set(str.str());
    }

    { //Crosshair Text
        auto thing = player.getPointedThing();

        std::ostringstream crossText;
        if (thing.blockDef != nullptr) crossText << thing.blockDef->getIdentifier() << std::endl;
        crosshairText.set(crossText.str());
    }
}

void DebugGui::draw(Renderer &renderer) {
    for (auto elem : children) {
        elem->draw(renderer);
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

    fpsGraph.setVisible(displayMode != 1);
}