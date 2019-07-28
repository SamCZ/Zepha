//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

DebugGui::DebugGui(glm::vec2 bufferSize, TextureAtlas& atlas) :
    displayMode(0)

//    coloredGraphTexture("../res/tex/gui/histogram.png"),
//    monochromeGraphTexture("../res/tex/gui/histogram_white.png")

//    chunkUpdatesGraph("Interp",     120, 128, &monochromeGraphTexture, &fontTexture),
//    meshUpdatesGraph ("ChunkMesh",  120, 128, &monochromeGraphTexture, &fontTexture),
//    serverGenGraph   ("Gen",        120, 512, &monochromeGraphTexture, &fontTexture),
//    serverPacketGraph("Packets",    120, 512, &monochromeGraphTexture, &fontTexture),
//
//    fpsGraph         ("FPS",        244, 64, 120, 60, &coloredGraphTexture   , &fontTexture),
//    drawCallsGraph   ("Draw Calls", 244, 64, 120, 0,  &monochromeGraphTexture, &fontTexture),
//    vRamGraph        ("VRam",       244, 64, 120, 1,  &monochromeGraphTexture, &fontTexture)
    {

    auto crosshairText = std::make_shared<GUIText>("crosshairText");
    crosshairText->create({2, 2}, {}, {0.1, 0.1, 0.1, 0.2}, {}, atlas.getTextureRef("font"));
    components.add(crosshairText);

    auto dataText = std::make_shared<GUIText>("dataText");
    dataText->create({2, 2}, {}, {0.1, 0.1, 0.1, 0.2}, {}, atlas.getTextureRef("font"));
    components.add(dataText);

    positionElements(bufferSize);

//    children.push_back(&dataText);
//    children.push_back(&crosshairText);
//
//    children.push_back(&chunkUpdatesGraph);
//    children.push_back(&meshUpdatesGraph);
//    children.push_back(&serverGenGraph);
//    children.push_back(&serverPacketGraph);
//    children.push_back(&fpsGraph);
//    children.push_back(&drawCallsGraph);
//    children.push_back(&vRamGraph);

//    children.push_back(&atlasTex);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth  = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    components.get<GUIText>("crosshairText")->setPos({bufferWidth / 2 + 22, bufferHeight / 2 - 7});
    components.get<GUIText>("dataText")->setPos(glm::vec3(10, 10, 0));

//    serverGenGraph.setPos({bufferWidth - 254, bufferHeight - 70 - 160});
//    serverPacketGraph.setPos({bufferWidth - 254, bufferHeight - 70 - 240});
//    meshUpdatesGraph.setPos({bufferWidth - 254, bufferHeight - 70 - 80});
//    chunkUpdatesGraph.setPos({bufferWidth - 254, bufferHeight - 70});
//    fpsGraph.setPos({10, bufferHeight - 70});
//    drawCallsGraph.setPos({10, bufferHeight - 70 - 80});
//    vRamGraph.setPos({bufferWidth - 254, 10});
}

void DebugGui::update(Player& player, LocalWorld& world, LocalDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {

    { //VRam Usage Graph (Top Right)
        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

//        vRamGraph.update(static_cast<int>(std::round(
//                (videoMemTotal - videoMemAvail) / static_cast<float>(videoMemTotal) * 100.0))
//                / 100.0f);
    }

    { //Bottom Left Graphs
//        fpsGraph.update(static_cast<float>(fps));
//        drawCallsGraph.update(drawCalls);
    }

    { //Bottom Right Graphs
//        meshUpdatesGraph.update(world.lastMeshUpdates);
//        chunkUpdatesGraph.update(world.lastGenUpdates);
//
//        serverGenGraph.update(static_cast<float>(ssGen));
//        serverPacketGraph.update(static_cast<float>(ssPack));
    }

    { //Top-left Data
        glm::vec3 footPos = TransPos::roundPos(player.getPos()) + glm::vec3(0, -2, 0);

        int blockID = world.getBlock(footPos);
        std::string on = (blockID > 0) ? defs.blocks().fromIndex(blockID).identifier : "invalid";

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
        std::string face =
            (thing.face == TOP)    ? "TOP" :
            (thing.face == BOTTOM) ? "BOTTOM" :
            (thing.face == LEFT)   ? "LEFT" :
            (thing.face == RIGHT)  ? "RIGHT" :
            (thing.face == FRONT)  ? "FRONT" :
            (thing.face == BACK)   ? "BACK" :
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

        if (thing.blockDef != nullptr) str << "Pointing At: " << thing.blockDef->identifier << ", " << std::endl;
        str << "Pointed Position: " << vecToString(thing.pos) << std::endl;
        str << "Pointed Face: " << face << std::endl;

        components.get<GUIText>("dataText")->setText(str.str());
    }

    { //Crosshair Text
        auto thing = player.getPointedThing();

        std::ostringstream crossText;
        if (thing.blockDef != nullptr) crossText << thing.blockDef->identifier << std::endl;
        components.get<GUIText>("crosshairText")->setText(crossText.str());
    }
}

void DebugGui::draw(Renderer &renderer) {
    components.draw(renderer);
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    components.setVisible(displayMode == 0);
//    components.get<GUILabelledGraph>("fpsGraph").setVisible(displayMode != 1);
}