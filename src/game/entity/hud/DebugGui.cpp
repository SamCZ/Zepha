//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"
#include "../../../util/TransPos.h"

DebugGui::DebugGui(glm::vec2 bufferSize, Texture* tex) {
    displayMode = 0;

    fontTexture = new Texture((char*)"../res/tex/gui/font.png");
    colorHistTexture = new Texture((char*)"../res/tex/gui/histogram.png");
    whiteHistTexture= new Texture((char*)"../res/tex/gui/histogram_white.png");
    transWhiteHistTexture= new Texture((char*)"../res/tex/gui/histogram_white_transparent.png");

    texAtlas = new RectEntity(tex);
    texAtlas->setScale({512, 512, 1});
    texAtlas->setPos({8, 300, 0});

    //Initialize GUI Objects

    dataText = new HudText(fontTexture);
    dataText->setScale(2);

    dataBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.4), glm::vec4(0.1, 0.1, 0.1, 0.4),
            glm::vec4(0.1, 0.1, 0.1, 0.3), glm::vec4(0.1, 0.1, 0.1, 0.3));
    dataBG->setScale(glm::vec3(560, 260, 1));

    crosshairText = new HudText(fontTexture);
    crosshairText->setScale(2);

    crosshairBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.3), glm::vec4(0.1, 0.1, 0.1, 0.3),
            glm::vec4(0.1, 0.1, 0.1, 0.5), glm::vec4(0.1, 0.1, 0.1, 0.5));
    crosshairBG->setScale(glm::vec3(244, 26, 1));

    chunkHist = new Histogram(whiteHistTexture, 120, 64, true);
    chunkHist->setScale(glm::vec3(2, 32, 1));

    chunkText = new HudText(fontTexture);
    chunkText->setScale(2);

    chunkBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    chunkBack->setScale(glm::vec3(244, 64, 1));

    meshHist = new Histogram(whiteHistTexture, 120, 64, true);
    meshHist->setScale(glm::vec3(2, 32, 1));

    meshText = new HudText(fontTexture);
    meshText->setScale(2);

    meshBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    meshBack->setScale(glm::vec3(244, 64, 1));

    ssGenHist = new Histogram(whiteHistTexture, 120, 64, true);
    ssGenHist->setScale(glm::vec3(2, 32, 1));

    ssGenText = new HudText(fontTexture);
    ssGenText->setScale(2);

    ssGenBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    ssGenBack->setScale(glm::vec3(244, 64, 1));

    ssPackHist = new Histogram(whiteHistTexture, 120, -1, true);
    ssPackHist->setScale(glm::vec3(2, 32, 1));

    ssPackText = new HudText(fontTexture);
    ssPackText->setScale(2);

    ssPackBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    ssPackBack->setScale(glm::vec3(244, 64, 1));

    fpsHist = new Histogram(colorHistTexture, 120, 60, true);
    fpsHist->setScale(glm::vec3(2, 20, 1));

    fpsBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    fpsBack->setScale(glm::vec3(244, 64, 1));

    fpsText = new HudText(fontTexture);
    fpsText->setScale(2);

    drawsHist = new Histogram(whiteHistTexture, 240, 1, false);
    drawsHist->setScale(glm::vec3(1, 20, 1));

    tMeshHist = new Histogram(transWhiteHistTexture, 240, 0, false);
    tMeshHist->setScale(glm::vec3(1, 20, 1));

    drawsBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    drawsBack->setScale(glm::vec3(244, 64, 1));

    drawsText = new HudText(fontTexture);
    drawsText->setScale(2);

    vramHist = new Histogram(whiteHistTexture, 240, 1, true);
    vramHist->setScale(glm::vec3(1, 32, 1));

    vramText = new HudText(fontTexture);
    vramText->setScale(2);

    vramBack = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    vramBack->setScale(glm::vec3(244, 64, 1));

    positionElements(bufferSize);

    addDrawable(dataBG);
    addDrawable(dataText);

    addDrawable(crosshairBG);
    addDrawable(crosshairText);

    addDrawable(chunkBack);
    addDrawable(chunkHist);
    addDrawable(chunkText);

    addDrawable(meshBack);
    addDrawable(meshHist);
    addDrawable(meshText);

    addDrawable(ssGenBack);
    addDrawable(ssGenHist);
    addDrawable(ssGenText);

    addDrawable(ssPackBack);
    addDrawable(ssPackHist);
    addDrawable(ssPackText);

    addDrawable(fpsBack);
    addDrawable(fpsHist);
    addDrawable(fpsText);

    addDrawable(drawsBack);
    addDrawable(drawsHist);
    addDrawable(tMeshHist);
    addDrawable(drawsText);

    addDrawable(vramBack);
    addDrawable(vramHist);
    addDrawable(vramText);

    addDrawable(texAtlas);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    glm::vec2 crosshairTextPos(bufferWidth / 2 + 22, bufferHeight / 2 - 7);

    crosshairText->setPos(glm::vec3(crosshairTextPos.x, crosshairTextPos.y, 0));
    crosshairBG->setPos(glm::vec3(crosshairTextPos.x - 4, crosshairTextPos.y - 4, -1));

    dataText->setPos(glm::vec3(10, 10, 0));
    dataBG->setPos(glm::vec3(6, 6, -1));

    glm::vec2 ssPackPos(bufferWidth - 254, bufferHeight - 70 - 240);
    glm::vec2 ssGenPos(bufferWidth - 254, bufferHeight - 70 - 160);
    glm::vec2 meshUpdatePos(bufferWidth - 254, bufferHeight - 70 - 80);
    glm::vec2 chunkUpdatePos(bufferWidth - 254, bufferHeight - 70);
    glm::vec2 fpsPos(10, bufferHeight - 70);
    glm::vec2 drawCallsPos(10, bufferHeight - 70 - 80);
    glm::vec2 videoMemoryPos(bufferWidth - 254, 10);

    ssGenText->setPos(glm::vec3(ssGenPos.x + 4, ssGenPos.y + 8, 0));
    ssGenHist->setPos(glm::vec3(ssGenPos.x + 2, ssGenPos.y + 60, -1));
    ssGenBack->setPos(glm::vec3(ssGenPos.x, ssGenPos.y, -2));

    ssPackText->setPos(glm::vec3(ssPackPos.x + 4, ssPackPos.y + 8, 0));
    ssPackHist->setPos(glm::vec3(ssPackPos.x + 2, ssPackPos.y + 60, -1));
    ssPackBack->setPos(glm::vec3(ssPackPos.x, ssPackPos.y, -2));

    meshText->setPos(glm::vec3(meshUpdatePos.x + 4, meshUpdatePos.y + 8, 0));
    meshHist->setPos(glm::vec3(meshUpdatePos.x + 2, meshUpdatePos.y + 60, -1));
    meshBack->setPos(glm::vec3(meshUpdatePos.x, meshUpdatePos.y, -2));

    chunkText->setPos(glm::vec3(chunkUpdatePos.x + 4, chunkUpdatePos.y + 8, 0));
    chunkHist->setPos(glm::vec3(chunkUpdatePos.x + 2, chunkUpdatePos.y + 60, -1));
    chunkBack->setPos(glm::vec3(chunkUpdatePos.x, chunkUpdatePos.y, -2));

    fpsText->setPos(glm::vec3(fpsPos.x + 4, fpsPos.y + 8, 0));
    fpsHist->setPos(glm::vec3(fpsPos.x + 2, fpsPos.y + 60, -1));
    fpsBack->setPos(glm::vec3(fpsPos.x, fpsPos.y, -2));

    drawsText->setPos(glm::vec3(drawCallsPos.x + 4, drawCallsPos.y + 8, 0));
    drawsHist->setPos(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    tMeshHist->setPos(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    drawsBack->setPos(glm::vec3(drawCallsPos.x, drawCallsPos.y, -2));

    vramText->setPos(glm::vec3(videoMemoryPos.x + 4, videoMemoryPos.y + 8, 0));
    vramHist->setPos(glm::vec3(videoMemoryPos.x + 2, videoMemoryPos.y + 60, -1));
    vramBack->setPos(glm::vec3(videoMemoryPos.x, videoMemoryPos.y, -2));
}

std::string string_float(float val) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    return out.str();
}

std::string string_float(double val) {
    return string_float((float)val);
}

std::string string_vec3(glm::vec3& vec) {
    std::ostringstream out;
    out << (int)vec.x << ", " << (int)vec.y << ", " << (int)vec.z;
    return out.str();
}

std::string string_vec3_float(glm::vec3& vec) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << vec.x << ", " << std::fixed << vec.y << ", " << std::fixed << vec.z;
    return out.str();
}

void DebugGui::update(Player& player, LocalWorld& world, GameDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {

    { //VRam Usage Graph (Top Right)
        int videoMemAvail, videoMemTotal;

        glGetIntegerv(0x9048, &videoMemTotal);
        glGetIntegerv(0x9049, &videoMemAvail);

        vramHist->setMax((float) videoMemTotal / 1024);
        vramHist->push_back((float) (videoMemTotal - videoMemAvail) / 1024);

        std::ostringstream str;

        str << "VRam Usage:" << std::endl;
        str << ((videoMemTotal - videoMemAvail) / 1024) << "MB ";
        str << "(" << ((int)std::round((videoMemTotal - videoMemAvail) / (float)videoMemTotal * 100.0)) << "%)";

        vramText->set(str.str());
    }

    { //Bottom Left Graphs
        fpsText->set("FPS: " + string_float(fps));
        fpsHist->push_back((float) fps);

        drawsHist->setMax(chunks);
        drawsHist->push_back(drawCalls);
        tMeshHist->push_back(chunks);
        drawsText->set("Draws: " + to_string(drawCalls) + ", " + to_string(chunks));
    }

    { //Bottom Right Graphs
        meshHist->push_back((float)world.lastMeshUpdates);
        meshText->set("Mesh: " + to_string(world.lastMeshUpdates));

        chunkHist->push_back((float)world.lastGenUpdates);
        chunkText->set("Interp: " + to_string(world.lastGenUpdates));

        ssGenHist->push_back((float)ssGen);
        ssGenText->set("Gen: " + to_string(ssGen));

        ssPackHist->push_back((float)ssPack);
        ssPackText->set("RX: " + to_string(ssPack));
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

        str << "Player: " << string_vec3(playerPos);
        str << " (" << string_vec3_float(*player.getPos()) << ")" << std::endl << std::endl;

        str << "Chunk: " << string_vec3(posOffsetFromChunk) << " [" << string_vec3(chunkPos) << "]" << std::endl;
        str << "MapBlock: " << string_vec3(posOffsetFromBlock) << std::endl;
        str << "Region: " << string_vec3(posOffsetFromRegion) << std::endl << std::endl;

        str << "Ch: " << string_vec3(chunkCoordinate) << ", ";
        str << "Mb: " << string_vec3(mapBlockCoordinate) << ", ";
        str << "Rg: " << string_vec3(regionCoordinate) << std::endl;

        str << "Vel: " << string_vec3_float(*player.getVel()) << std::endl;
        str << "Yaw: " << string_float(player.getYaw()) << ", ";
        str << "Pitch: " << string_float(player.getPitch()) << std::endl << std::endl;

        str << "Standing On: " << on << std::endl << std::endl;

        str << "Pointing: " << (player.pointingAtBlock ? "yes" : "no") << ", ";
        str << "PointedBlock: " << string_vec3(player.pointedBlock) << std::endl;
        str << "BreakState: " << string_float(player.digPercentage);

        dataText->set(str.str());
    }

    { //Crosshair Text
        std::string look;
        auto block = player.getPointedBlock();
        if (block != nullptr) {
            look = defs.blocks().getBlock(world.getBlock(*block)).getIdentifier();
        }
        else {
            look = "invalid";
        }

        crosshairText->set(look);
        auto scale = crosshairBG->getScale();
        scale.x = look.size() * 7 * 2 + 10;
        crosshairBG->setScale(scale);
    }
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    for (auto elem : children) {
        ((Entity*)elem)->setVisible(displayMode == 0);
    }

    fpsText->setVisible(displayMode != 1);
    fpsHist->setVisible(displayMode != 1);
    fpsBack->setVisible(displayMode != 1);
}