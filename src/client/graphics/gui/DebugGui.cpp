//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"
#include "../../engine/graphics/Histogram.h"

DebugGui::DebugGui() {
    fontTexture = new Texture((char*)"../res/tex/gui/font.png");
    fontTexture->load();

    fpsHist = new Texture((char*)"../res/tex/gui/histogram.png");
    fpsHist->load();
    genHist = new Texture((char*)"../res/tex/gui/histogram_white.png");
    genHist->load();

    alphaText = new HudText(fontTexture);
    alphaText->set("Zeus Alpha");
    alphaText->setScale(3);
    alphaText->setPosition(glm::vec3(8, 4, 0));

    fpsText = new HudText(fontTexture);
    fpsText->setScale(2);
    fpsText->setPosition(glm::vec3(16, 740, 0));

    playerText = new HudText(fontTexture);
    playerText->setScale(2);
    playerText->setPosition(glm::vec3(8, 42, 0));

    chunkUpdateHistogram = new Histogram(genHist, 120, 64, true);
    chunkUpdateHistogram->setScale(glm::vec3(2, 32, 1));
    chunkUpdateHistogram->setPosition(glm::vec3(870, 760, 0));

    meshUpdateHistogram = new Histogram(genHist, 120, 64, true);
    meshUpdateHistogram->setScale(glm::vec3(2, 32, 1));
    meshUpdateHistogram->setPosition(glm::vec3(1118, 760, 0));

    fpsHistogram = new Histogram(fpsHist, 120, 60, true);
    fpsHistogram->setScale(glm::vec3(2, 20, 1));
    fpsHistogram->setPosition(glm::vec3(8, 760, 0));
}

void DebugGui::pushGuiObjects(std::vector<Entity*> &list) {
    list.push_back(alphaText);
//    list.push_back(fpsText);
    list.push_back(playerText);

    list.push_back(chunkUpdateHistogram);
    list.push_back(meshUpdateHistogram);
    list.push_back(fpsHistogram);
}

std::string string_float(float val) {
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    std::string s = out.str();
    return s;
}

std::string string_double(double val) {
    return string_float((float)val);
}

void DebugGui::update(Player* player, World* world, Window* window, BlockAtlas* atlas, double fps) {
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

    fpsText->set(string_double(fps) + " FPS");
    fpsHistogram->push_back((float)fps);

    meshUpdateHistogram->push_back((float)world->lastMeshUpdates);
    chunkUpdateHistogram->push_back((float)world->lastGenUpdates);

    using namespace std; //Temporary, delete when refactoring this
    playerText->set(
            "Chunk: " + to_string(world->lastGenUpdates) + ", Mesh: " + to_string(world->lastMeshUpdates) + "\n" +
            "W: " + to_string((int)player->getPos()->x) + "," + to_string((int)player->getPos()->y) + "," + to_string((int)player->getPos()->z) + "\n" +
            "C: " + to_string((int)chk.x) + "," + to_string((int)chk.y) + "," + to_string((int)chk.z) + " " +
            "(" + to_string((int)loc.x) + "," + to_string((int)loc.y) + "," + to_string((int)loc.z) + ")\n" +
            "V: " + string_float(player->getVel()->x) + "," + string_float(player->getVel()->y) + "," + string_float(player->getVel()->z) + "\n" +
            "Yaw: " + string_float(player->getYaw()) + ", Pitch: " + string_float(player->getPitch()) + "\n" +
            "On: " + on + "\nLooking: " + look);
}

DebugGui::~DebugGui() = default;