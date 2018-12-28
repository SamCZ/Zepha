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

    //The world requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    renderer->getCamera()->setPosition(glm::vec3(8, 24, 8));

    int SIZE = 10;
    for (int i = -SIZE; i < SIZE; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = -SIZE; k < SIZE; k++) {
                world->genNewChunk(glm::vec3(i, j, k));
            }
        }
    }

    world->genNewChunk(glm::vec3(0, 0, 0));

    gui.pushGuiObjects(guiEntities);
}

void GameInstance::update(GLfloat deltaTime, double fps) {
    renderer->update();

    auto camera = renderer->getCamera();
    auto window = renderer->getWindow();

    camera->keyControl(window->getKeysArray(), deltaTime);
    camera->mouseControl(window->getDeltaX(), window->getDeltaY());

    glm::vec3 round = World::roundVec(*camera->getPosition());
    round.y -= 2;
//    glm::vec3 chunk = World::chunkVec(round);
//    glm::vec3 local = World::localVec(round);

    int block = world->getBlock(round);
    std::string name = "Null";
    if (block >= 0) {
        name = blockAtlas->getBlock(block)->getIdentifier();
    }

    gui.update(round, name, fps);

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

    Texture* prevTexture = nullptr;

    for (auto &gui : guiEntities) {
        if (gui->getTexture() != nullptr && gui->getTexture() != prevTexture) {
            prevTexture = gui->getTexture();
            gui->getTexture()->use();
        }

        renderer->drawGui(gui);
    }

    renderer->end();
}
