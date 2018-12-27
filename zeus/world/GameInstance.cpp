//
// Created by aurailus on 17/12/18.
//

#include "GameInstance.h"
#include "../lua_api/LRegisterBlock.h"

GameInstance::GameInstance() = default;

void GameInstance::initialize(Renderer* renderer) {
    this->renderer = renderer;

    textureAtlas = new TextureAtlas("../tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();
    auto Z = p.getModule();

    //Register APIs here
    LRegisterBlock(this).regApi(Z);

    p.doFile("../lua/file.lua");

    //Build the world
    //The world requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    renderer->getCamera()->setPosition(glm::vec3(8, 24, 8));

//    int SIZE = 24;
//    for (int i = -SIZE; i < SIZE; i++) {
//        for (int j = 0; j < 3; j++) {
//            for (int k = -SIZE; k < SIZE; k++) {
//                world->genNewChunk(glm::vec3(i, j, k));
//            }
//        }
//    }

    world->genNewChunk(glm::vec3(0, 0, 0));

    fontTexture = Texture((char*)"../tex/font.png");
    fontTexture.load();

    alphaText = new HudText();
    alphaText->set("Zeus Alpha");
    alphaText->setScale(2.5);
    alphaText->setPosition(glm::vec3(8, 4, 0));
    guiEntities.push_back(alphaText);

    fpsText = new HudText();
    fpsText->setScale(2);
    fpsText->setPosition(glm::vec3(8, 32, 0));
    guiEntities.push_back(fpsText);

    blockText = new HudText();
    blockText->setScale(2);
    blockText->setPosition(glm::vec3(8, 52, 0));
    guiEntities.push_back(blockText);
}

void GameInstance::update(GLfloat deltaTime) {
    renderer->update();

    auto camera = renderer->getCamera();
    auto window = renderer->getWindow();

    camera->keyControl(window->getKeysArray(), deltaTime);
    camera->mouseControl(window->getDeltaX(), window->getDeltaY());

    glm::vec3 round = World::roundVec(*camera->getPosition());
    round.y -= 2;
    glm::vec3 chunk = World::chunkVec(round);
    glm::vec3 local = World::localVec(round);

    int block = world->getBlock(round);
    std::string name = "Null";

    if (block >= 0) {
        name = blockAtlas->getBlock(block)->getIdentifier();
    }

    if (block < 1) {
        camera->getPosition()->y -= 0.1;
    }

    blockText->set(
        "World Pos: (" + to_string((int)round.x) + ", " + to_string((int)round.y) + ", " + to_string((int)round.z) + ")\n" +
        "Chunk Pos: (" + to_string((int)chunk.x) + ", " + to_string((int)chunk.y) + ", " + to_string((int)chunk.z) + ")\n" +
        "Local Pos: (" + to_string((int)local.x) + ", " + to_string((int)local.y) + ", " + to_string((int)local.z) + ")\n" +
        "Block: " + name);
    world->update();
}

void GameInstance::draw() {
    Timer t("Drawing");

    renderer->begin();

    renderer->enableWorldShader();
    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks()) {
        renderer->draw(chunk.second);
    }

    renderer->enableGuiShader();
    fontTexture.use();

    for (auto &gui : *getGuiEntities()) {
        renderer->drawGui(gui);
    }

    renderer->end();
}

std::vector<Entity*>* GameInstance::getGuiEntities() {
    return &guiEntities;
}
