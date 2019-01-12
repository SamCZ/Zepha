//
// Created by aurailus on 27/12/18.
//

#include "DebugGui.h"

DebugGui::DebugGui() {
    fontTexture = new Texture((char*)"../tex/gui/font.png");
    fontTexture->load();

    histogramTexture = new Texture((char*)"../tex/gui/histogram.png");
    histogramTexture->load();

    alphaText = new HudText(fontTexture);
    alphaText->set("Zeus Alpha 0.01");
    alphaText->setScale(3);
    alphaText->setPosition(glm::vec3(8, 4, 0));

    fpsText = new HudText(fontTexture);
    fpsText->setScale(2);
    fpsText->setPosition(glm::vec3(16, 740, 0));

    playerText = new HudText(fontTexture);
    playerText->setScale(2);
    playerText->setPosition(glm::vec3(8, 42, 0));

    fpsHistogram = new Entity();
    fpsHistogram->create(new Mesh(), histogramTexture);
    fpsHistogram->setPosition(glm::vec3(8, 764, 0));
}

void DebugGui::pushGuiObjects(std::vector<Entity*> &list) {
    list.push_back(alphaText);
    list.push_back(fpsText);
    list.push_back(playerText);
    list.push_back(fpsHistogram);
}

void DebugGui::fpsHistUpdate() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int indOffset = 0;
    float xOffset = 0;
    float width = 6;
    float height = 0.5;
    float i = 0.10;

    for (double num : fpsHistory) {
        float h = (float)num * height;
        float sev = round(9 - (float)max(60 - num, 0.0)/6) / 10.0f;

        auto columnVerts = std::vector<float> {
                xOffset,         -h, 0,  sev  , sev,   0, 0, 0,
                xOffset + width, -h, 0,  sev+i, sev,   0, 0, 0,
                xOffset + width,  0, 0,  sev+i, sev+i, 0, 0, 0,
                xOffset,          0, 0,  sev  , sev+i, 0, 0, 0,
        };

        for (float f : columnVerts) vertices.push_back(f);

        indices.push_back(    indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(1 + indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(2 + indOffset);
        indices.push_back(1 + indOffset);

        xOffset += width;
        indOffset += 4;
    }

    auto m = new Mesh();
    m->create(&vertices, &indices);

    fpsHistogram->cleanup();
    fpsHistogram->create(m);
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

    if (fpsHistory.size() > FPS_HISTOGRAM_SIZE) fpsHistory.erase(fpsHistory.begin());

    fpsHistory.push_back(fps);
    fpsText->set(string_double(fps) + " FPS");
    fpsHistUpdate();

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