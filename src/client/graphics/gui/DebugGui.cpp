//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"
#include "../../engine/graphics/Histogram.h"

DebugGui::DebugGui(glm::vec2 bufferSize) {
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
    dataBG->setScale(glm::vec3(400, 154, 1));

    crosshairText = new HudText(fontTexture);
    crosshairText->setScale(2);

    crosshairBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.3), glm::vec4(0.1, 0.1, 0.1, 0.3),
            glm::vec4(0.1, 0.1, 0.1, 0.5), glm::vec4(0.1, 0.1, 0.1, 0.5));
    crosshairBG->setScale(glm::vec3(244, 26, 1));

    chunkUpdateHistogram = new Histogram(whiteHistTexture, 120, 64, true);
    chunkUpdateHistogram->setScale(glm::vec3(2, 32, 1));

    chunkUpdateText = new HudText(fontTexture);
    chunkUpdateText->setScale(2);

    chunkUpdateBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    chunkUpdateBG->setScale(glm::vec3(244, 64, 1));

    meshUpdateHistogram = new Histogram(whiteHistTexture, 120, 64, true);
    meshUpdateHistogram->setScale(glm::vec3(2, 32, 1));

    meshUpdateText = new HudText(fontTexture);
    meshUpdateText->setScale(2);

    meshUpdateBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    meshUpdateBG->setScale(glm::vec3(244, 64, 1));

    fpsHistogram = new Histogram(colorHistTexture, 120, 60, true);
    fpsHistogram->setScale(glm::vec3(2, 20, 1));

    fpsBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    fpsBG->setScale(glm::vec3(244, 64, 1));

    fpsText = new HudText(fontTexture);
    fpsText->setScale(2);

    drawCallsHistogram = new Histogram(whiteHistTexture, 240, 1, false);
    drawCallsHistogram->setScale(glm::vec3(1, 20, 1));

    chunkHistogram = new Histogram(transWhiteHistTexture, 240, 0, false);
    chunkHistogram->setScale(glm::vec3(1, 20, 1));

    drawCallsBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    drawCallsBG->setScale(glm::vec3(244, 64, 1));

    drawCallsText = new HudText(fontTexture);
    drawCallsText->setScale(2);

    videoMemoryHistogram = new Histogram(whiteHistTexture, 240, 1, true);
    videoMemoryHistogram->setScale(glm::vec3(1, 32, 1));

    videoMemoryText = new HudText(fontTexture);
    videoMemoryText->setScale(2);

    videoMemoryBG = new RectEntity(
            glm::vec4(0.1, 0.1, 0.1, 0.2), glm::vec4(0.1, 0.1, 0.1, 0.2),
            glm::vec4(0.1, 0.1, 0.1, 0.7), glm::vec4(0.1, 0.1, 0.1, 0.7));
    videoMemoryBG->setScale(glm::vec3(244, 64, 1));

    positionElements(bufferSize);
}

void DebugGui::positionElements(glm::vec2 bufferSize) {
    auto bufferWidth = (int)bufferSize.x;
    auto bufferHeight = (int)bufferSize.y;

    glm::vec2 crosshairTextPos(bufferWidth / 2 + 22, bufferHeight / 2 - 7);

    crosshairText->setPosition(glm::vec3(crosshairTextPos.x, crosshairTextPos.y, 0));
    crosshairBG->setPosition(glm::vec3(crosshairTextPos.x - 4, crosshairTextPos.y - 4, -1));

    dataText->setPosition(glm::vec3(10, 10, 0));
    dataBG->setPosition(glm::vec3(6, 6, -1));

    glm::vec2 meshUpdatePos(bufferWidth - 254, bufferHeight - 70 - 80);
    glm::vec2 chunkUpdatePos(bufferWidth - 254, bufferHeight - 70);
    glm::vec2 fpsPos(10, bufferHeight - 70);
    glm::vec2 drawCallsPos(10, bufferHeight - 70 - 80);
    glm::vec2 videoMemoryPos(bufferWidth - 254, 10);

    meshUpdateText->setPosition(glm::vec3(meshUpdatePos.x + 4, meshUpdatePos.y + 8, 0));
    meshUpdateHistogram->setPosition(glm::vec3(meshUpdatePos.x + 2, meshUpdatePos.y + 60, -1));
    meshUpdateBG->setPosition(glm::vec3(meshUpdatePos.x, meshUpdatePos.y, -2));

    chunkUpdateText->setPosition(glm::vec3(chunkUpdatePos.x + 4, chunkUpdatePos.y + 8, 0));
    chunkUpdateHistogram->setPosition(glm::vec3(chunkUpdatePos.x + 2, chunkUpdatePos.y + 60, -1));
    chunkUpdateBG->setPosition(glm::vec3(chunkUpdatePos.x, chunkUpdatePos.y, -2));

    fpsText->setPosition(glm::vec3(fpsPos.x + 4, fpsPos.y + 8, 0));
    fpsHistogram->setPosition(glm::vec3(fpsPos.x + 2, fpsPos.y + 60, -1));
    fpsBG->setPosition(glm::vec3(fpsPos.x, fpsPos.y, -2));

    drawCallsText->setPosition(glm::vec3(drawCallsPos.x + 4, drawCallsPos.y + 8, 0));
    drawCallsHistogram->setPosition(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    chunkHistogram->setPosition(glm::vec3(drawCallsPos.x + 2, drawCallsPos.y + 60, -1));
    drawCallsBG->setPosition(glm::vec3(drawCallsPos.x, drawCallsPos.y, -2));

    videoMemoryText->setPosition(glm::vec3(videoMemoryPos.x + 4, videoMemoryPos.y + 8, 0));
    videoMemoryHistogram->setPosition(glm::vec3(videoMemoryPos.x + 2, videoMemoryPos.y + 60, -1));
    videoMemoryBG->setPosition(glm::vec3(videoMemoryPos.x, videoMemoryPos.y, -2));
}

void DebugGui::pushGuiObjects(std::vector<Entity*> &list) {
    list.push_back(dataBG);
    list.push_back(dataText);

    list.push_back(crosshairBG);
    list.push_back(crosshairText);

    list.push_back(chunkUpdateBG);
    list.push_back(chunkUpdateHistogram);
    list.push_back(chunkUpdateText);

    list.push_back(meshUpdateBG);
    list.push_back(meshUpdateHistogram);
    list.push_back(meshUpdateText);

    list.push_back(fpsBG);
    list.push_back(fpsHistogram);
    list.push_back(fpsText);

    list.push_back(drawCallsBG);
    list.push_back(drawCallsHistogram);
    list.push_back(chunkHistogram);
    list.push_back(drawCallsText);

    list.push_back(videoMemoryBG);
    list.push_back(videoMemoryHistogram);
    list.push_back(videoMemoryText);
}

std::string string_float(float val) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    std::string s = out.str();
    return s;
}

void DebugGui::update(Player* player, LocalWorld* world, Window* window, BlockAtlas* atlas, double fps, int chunks, int drawCalls) {
    using namespace std;

    glGetIntegerv(0x9048, &videoMemTotal);
    glGetIntegerv(0x9049, &videoMemAvail);

    glm::vec3 round = LocalWorld::roundVec(*player->getPos());
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
            if (window->mouseIsDown()) {
                world->setBlock(*ray.getEnd(), 0);
            }
            break;
        }
    }

    std::string look = "ignore";
    if (block >= 0) {
        look = atlas->getBlock(block)->getIdentifier();
    }

    glm::vec3 chk = LocalWorld::chunkVec(*player->getPos());
    glm::vec3 loc = LocalWorld::localVec(*player->getPos());

    fpsText->set("FPS:" + string_float((float)fps));
    fpsHistogram->push_back((float)fps);

    drawCallsText->set("MCD:" + to_string(drawCalls) + "," + to_string(chunks));
    chunkHistogram->push_back(chunks);
    drawCallsHistogram->setMax(chunks);
    drawCallsHistogram->push_back(drawCalls);

    meshUpdateHistogram->push_back((float)world->lastMeshUpdates);
    meshUpdateText->set("Mesh:" + to_string(world->lastMeshUpdates));

    chunkUpdateHistogram->push_back((float)world->lastGenUpdates);
    chunkUpdateText->set("Gen:" + to_string(world->lastGenUpdates));

    videoMemoryHistogram->setMax((float)videoMemTotal / 1024);
    videoMemoryHistogram->push_back((float)(videoMemTotal - videoMemAvail) / 1024);
    videoMemoryText->set("Total VRam Usage:\n" + to_string((videoMemTotal - videoMemAvail) / 1024) + "MB, (" + to_string((int)std::round((videoMemTotal - videoMemAvail) / (float)videoMemTotal * 100.0f)) + "%)");

    dataText->set(
            "World: " + to_string((int)player->getPos()->x) + "," + to_string((int)player->getPos()->y) + "," + to_string((int)player->getPos()->z) + "\n" +
            "Local: " + to_string((int)loc.x) + "," + to_string((int)loc.y) + "," + to_string((int)loc.z) + "\n" +
            "Chunk: " + to_string((int)chk.x) + "," + to_string((int)chk.y) + "," + to_string((int)chk.z) + "\n\n" +
            "Vel: " + string_float(player->getVel()->x) + "," + string_float(player->getVel()->y) + "," + string_float(player->getVel()->z) + "\n" +
            "Yaw: " + string_float(player->getYaw()) + ", Pitch: " + string_float(player->getPitch()) + "\n\n" +
            "Standing On: " + on);

    crosshairText->set(look);
    auto crosshairScale = crosshairBG->getScale();
    crosshairScale->x = look.size() * 7 * 2 + 10;
}

void DebugGui::bufferResized(glm::vec2 bufferSize) {
    positionElements(bufferSize);
}

DebugGui::~DebugGui() = default;