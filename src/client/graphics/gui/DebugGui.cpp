//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"
#include "../../../generic/helpers/TransPos.h"

DebugGui::DebugGui(glm::vec2 bufferSize) {
    displayMode = 0;

    fontTexture = new Texture((char*)"../res/tex/gui/font.png");
    fontTexture->load();

    colorHistTexture = new Texture((char*)"../res/tex/gui/histogram.png");
    colorHistTexture->load();

    whiteHistTexture= new Texture((char*)"../res/tex/gui/histogram_white.png");
    whiteHistTexture->load();

    transWhiteHistTexture= new Texture((char*)"../res/tex/gui/histogram_white_transparent.png");
    transWhiteHistTexture->load();

    //Initialize GUI Objects

    dataText = new HudText(fontTexture);
    dataText->setScale(2);

    dataBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.4), glm::vec4(0.1, 0.1, 0.1, 0.4),
            glm::vec4(0.1, 0.1, 0.1, 0.3), glm::vec4(0.1, 0.1, 0.1, 0.3));
    dataBG->setScale(glm::vec3(560, 168, 1));

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
    
    ownedElements.push_back(dataBG);
    ownedElements.push_back(dataText);

    ownedElements.push_back(crosshairBG);
    ownedElements.push_back(crosshairText);

    ownedElements.push_back(chunkBack);
    ownedElements.push_back(chunkHist);
    ownedElements.push_back(chunkText);

    ownedElements.push_back(meshBack);
    ownedElements.push_back(meshHist);
    ownedElements.push_back(meshText);

    ownedElements.push_back(ssGenBack);
    ownedElements.push_back(ssGenHist);
    ownedElements.push_back(ssGenText);

    ownedElements.push_back(ssPackBack);
    ownedElements.push_back(ssPackHist);
    ownedElements.push_back(ssPackText);

    ownedElements.push_back(fpsBack);
    ownedElements.push_back(fpsHist);
    ownedElements.push_back(fpsText);

    ownedElements.push_back(drawsBack);
    ownedElements.push_back(drawsHist);
    ownedElements.push_back(tMeshHist);
    ownedElements.push_back(drawsText);

    ownedElements.push_back(vramBack);
    ownedElements.push_back(vramHist);
    ownedElements.push_back(vramText);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    glm::vec2 crosshairTextPos(bufferWidth / 2 + 22, bufferHeight / 2 - 7);

    crosshairText->setPosition(glm::vec3(crosshairTextPos.x, crosshairTextPos.y, 0));
    crosshairBG->setPosition(glm::vec3(crosshairTextPos.x - 4, crosshairTextPos.y - 4, -1));

    dataText->setPosition(glm::vec3(10, 10, 0));
    dataBG->setPosition(glm::vec3(6, 6, -1));

    glm::vec2 ssPackPos(bufferWidth - 254, bufferHeight - 70 - 240);
    glm::vec2 ssGenPos(bufferWidth - 254, bufferHeight - 70 - 160);
    glm::vec2 meshUpdatePos(bufferWidth - 254, bufferHeight - 70 - 80);
    glm::vec2 chunkUpdatePos(bufferWidth - 254, bufferHeight - 70);
    glm::vec2 fpsPos(10, bufferHeight - 70);
    glm::vec2 drawCallsPos(10, bufferHeight - 70 - 80);
    glm::vec2 videoMemoryPos(bufferWidth - 254, 10);

    ssGenText->setPosition(glm::vec3(ssGenPos.x + 4, ssGenPos.y + 8, 0));
    ssGenHist->setPosition(glm::vec3(ssGenPos.x + 2, ssGenPos.y + 60, -1));
    ssGenBack->setPosition(glm::vec3(ssGenPos.x, ssGenPos.y, -2));

    ssPackText->setPosition(glm::vec3(ssPackPos.x + 4, ssPackPos.y + 8, 0));
    ssPackHist->setPosition(glm::vec3(ssPackPos.x + 2, ssPackPos.y + 60, -1));
    ssPackBack->setPosition(glm::vec3(ssPackPos.x, ssPackPos.y, -2));

    meshText->setPosition(glm::vec3(meshUpdatePos.x + 4, meshUpdatePos.y + 8, 0));
    meshHist->setPosition(glm::vec3(meshUpdatePos.x + 2, meshUpdatePos.y + 60, -1));
    meshBack->setPosition(glm::vec3(meshUpdatePos.x, meshUpdatePos.y, -2));

    chunkText->setPosition(glm::vec3(chunkUpdatePos.x + 4, chunkUpdatePos.y + 8, 0));
    chunkHist->setPosition(glm::vec3(chunkUpdatePos.x + 2, chunkUpdatePos.y + 60, -1));
    chunkBack->setPosition(glm::vec3(chunkUpdatePos.x, chunkUpdatePos.y, -2));

    fpsText->setPosition(glm::vec3(fpsPos.x + 4, fpsPos.y + 8, 0));
    fpsHist->setPosition(glm::vec3(fpsPos.x + 2, fpsPos.y + 60, -1));
    fpsBack->setPosition(glm::vec3(fpsPos.x, fpsPos.y, -2));

    drawsText->setPosition(glm::vec3(drawCallsPos.x + 4, drawCallsPos.y + 8, 0));
    drawsHist->setPosition(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    tMeshHist->setPosition(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    drawsBack->setPosition(glm::vec3(drawCallsPos.x, drawCallsPos.y, -2));

    vramText->setPosition(glm::vec3(videoMemoryPos.x + 4, videoMemoryPos.y + 8, 0));
    vramHist->setPosition(glm::vec3(videoMemoryPos.x + 2, videoMemoryPos.y + 60, -1));
    vramBack->setPosition(glm::vec3(videoMemoryPos.x, videoMemoryPos.y, -2));
}

void DebugGui::pushGuiObjects(std::vector<Entity*> &list) {
    for (auto element : ownedElements) {
        list.push_back(element);
    }
}

std::string string_float(float val) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    std::string s = out.str();
    return s;
}

void DebugGui::update(Player* player, LocalWorld* world, Window* window, BlockAtlas* atlas, double fps, int chunks, int drawCalls, int ssGen, int ssPack) {
    using namespace std;

    glGetIntegerv(0x9048, &videoMemTotal);
    glGetIntegerv(0x9049, &videoMemAvail);

    glm::vec3 round = TransPos::roundPos(*player->getPos());
    round.y -= 2;

    int block = world->getBlock(round);
    std::string on = "ignore";
    if (block >= 0) {
        on = atlas->getBlock(block)->getIdentifier();
    }

    block = 0;
    for (Ray ray(player); ray.getLength() < 5; ray.step(0.01)) {
        auto found = world->getBlock(*ray.getEnd());
        if (found > 0) {
            block = found;
            break;
        }
    }

    std::string look = "ignore";
    if (block >= 0) {
        look = atlas->getBlock(block)->getIdentifier();
    }

    fpsText->set("FPS: " + string_float((float)fps));
    fpsHist->push_back((float)fps);

    drawsText->set("Draws: " + to_string(drawCalls) + ", " + to_string(chunks));
    tMeshHist->push_back(chunks);
    drawsHist->setMax(chunks);
    drawsHist->push_back(drawCalls);

    meshHist->push_back((float)world->lastMeshUpdates);
    meshText->set("Mesh: " + to_string(world->lastMeshUpdates));

    chunkHist->push_back((float)world->lastGenUpdates);
    chunkText->set("Interp: " + to_string(world->lastGenUpdates));

    ssGenHist->push_back((float)ssGen);
    ssGenText->set("Gen: " + to_string(ssGen));

    ssPackHist->push_back((float)ssPack);
    ssPackText->set("RX: " + to_string(ssPack));

    vramHist->setMax((float)videoMemTotal / 1024);
    vramHist->push_back((float)(videoMemTotal - videoMemAvail) / 1024);
    vramText->set("Total VRam Usage:\n" + to_string((videoMemTotal - videoMemAvail) / 1024) + "MB, (" + to_string((int)std::round((videoMemTotal - videoMemAvail) / (float)videoMemTotal * 100.0f)) + "%)");

    glm::vec3* ppos = player->getPos();
    glm::vec3 rpos = TransPos::roundPos(*ppos);
    glm::vec3 chk = TransPos::chunkFromGlobal(*player->getPos());
    glm::vec3 loc = TransPos::chunkLocalFromGlobal(*player->getPos());
    glm::vec3 mpr = TransPos::mapBlockFromGlobal(*player->getPos());
    glm::vec3 mp = TransPos::mapBlockLocalFromGlobal(*player->getPos());
    glm::vec3 rpr = TransPos::regionFromGlobal(*player->getPos());
    glm::vec3 rp = TransPos::regionLocalFromGlobal(*player->getPos());

    dataText->set(
            "Player: " + to_string((int)rpos.x) + ", " + to_string((int)rpos.y) + ", " + to_string((int)rpos.z) +
                   " (" + string_float(ppos->x) + ", " + string_float(ppos->y) + ", " + string_float(ppos->z) + ")\n" +
            "Chunk: " + to_string((int)loc.x) + ", " + to_string((int)loc.y) + ", " + to_string((int)loc.z) +
                   " (" + to_string((int)chk.x) + ", " + to_string((int)chk.y) + ", " + to_string((int)chk.z) + ")\n" +
            "MapBlock: " + to_string((int)mp.x) + ", " + to_string((int)mp.y) + ", " + to_string((int)mp.z) +
                   " (" + to_string((int)mpr.x) + ", " + to_string((int)mpr.y) + ", " + to_string((int)mpr.z) + ")\n" +
            "Region: " + to_string((int)rp.x) + ", " + to_string((int)rp.y) + ", " + to_string((int)rp.z) +
                   " (" + to_string((int)rpr.x) + ", " + to_string((int)rpr.y) + ", " + to_string((int)rpr.z) + ")\n\n" +
            "Vel: " + string_float(player->getVel()->x) + ", " + string_float(player->getVel()->y) + ", " + string_float(player->getVel()->z) + "\n" +
            "Yaw: " + string_float(player->getYaw()) + ", Pitch: " + string_float(player->getPitch()) + "\n\n" +
            "Standing On: " + on);

    crosshairText->set(look);
    auto crosshairScale = crosshairBG->getScale();
    crosshairScale->x = look.size() * 7 * 2 + 10;
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

//0 = All, 1 = None, 2 = FPS
void DebugGui::changeVisibilityState(int state) {
    displayMode = state;

    for (auto elem : ownedElements) {
        elem->setVisible(displayMode == 0);
    }

    fpsText->setVisible(displayMode != 1);
    fpsHist->setVisible(displayMode != 1);
    fpsBack->setVisible(displayMode != 1);
}

DebugGui::~DebugGui() = default;