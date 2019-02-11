//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"
#include "../../engine/graphics/Histogram.h"

DebugGui::DebugGui() {
    fontTexture = new Texture((char*)"../res/tex/gui/font.png");
    fontTexture->load();

    colorHistTexture = new Texture((char*)"../res/tex/gui/histogram.png");
    colorHistTexture->load();

    whiteHistTexture= new Texture((char*)"../res/tex/gui/histogram_white.png");
    whiteHistTexture->load();

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

    positionElements(1920, 1000);
}

void DebugGui::positionElements(int bufferWidth, int bufferHeight) {
    glm::vec2 crosshairTextPos(bufferWidth / 2 + 22, bufferHeight / 2 - 7);

    crosshairText->setPosition(glm::vec3(crosshairTextPos.x, crosshairTextPos.y, 0));
    crosshairBG->setPosition(glm::vec3(crosshairTextPos.x - 4, crosshairTextPos.y - 4, -1));

    dataText->setPosition(glm::vec3(10, 10, 0));
    dataBG->setPosition(glm::vec3(6, 6, -1));

    glm::vec2 meshUpdatePos(bufferWidth - 254, bufferHeight - 70 - 80);
    glm::vec2 chunkUpdatePos(bufferWidth - 254, bufferHeight - 70);
    glm::vec2 fpsPos(10, bufferHeight - 70);

    meshUpdateText->setPosition(glm::vec3(meshUpdatePos.x + 4, meshUpdatePos.y + 8, 0));
    meshUpdateHistogram->setPosition(glm::vec3(meshUpdatePos.x + 2, meshUpdatePos.y + 60, -1));
    meshUpdateBG->setPosition(glm::vec3(meshUpdatePos.x, meshUpdatePos.y, -2));

    chunkUpdateText->setPosition(glm::vec3(chunkUpdatePos.x + 4, chunkUpdatePos.y + 8, 0));
    chunkUpdateHistogram->setPosition(glm::vec3(chunkUpdatePos.x + 2, chunkUpdatePos.y + 60, -1));
    chunkUpdateBG->setPosition(glm::vec3(chunkUpdatePos.x, chunkUpdatePos.y, -2));

    fpsText->setPosition(glm::vec3(fpsPos.x + 4, fpsPos.y + 8, 0));
    fpsHistogram->setPosition(glm::vec3(fpsPos.x + 2, fpsPos.y + 60, -1));
    fpsBG->setPosition(glm::vec3(fpsPos.x, fpsPos.y, -2));
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
}

std::string string_float(float val) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    std::string s = out.str();
    return s;
}

void DebugGui::update(Player* player, World* world, Window* window, BlockAtlas* atlas, double fps) {
    using namespace std;

    glm::vec3 round = World::roundVec(*player->getPos());
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

    glm::vec3 chk = World::chunkVec(*player->getPos());
    glm::vec3 loc = World::localVec(*player->getPos());

    fpsText->set("FPS:" + string_float((float)fps));
    fpsHistogram->push_back((float)fps);

    meshUpdateHistogram->push_back((float)world->lastMeshUpdates);
    meshUpdateText->set("Mesh:" + to_string(world->lastMeshUpdates));

    chunkUpdateHistogram->push_back((float)world->lastGenUpdates);
    chunkUpdateText->set("Gen:" + to_string(world->lastGenUpdates));

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

DebugGui::~DebugGui() = default;