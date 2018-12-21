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

//    world->genNewChunk(glm::vec3(0,0,0));

    int SIZE = 16;
    for (int i = -SIZE; i < SIZE; i++) {
        for (int j = -4; j < 4; j++) {
            for (int k = -SIZE; k < SIZE; k++) {
                world->genNewChunk(glm::vec3(i, j, k));
            }
        }
    }
}

void GameInstance::update(GLfloat deltaTime) {
    auto camera = renderer->getCamera();
    auto window = renderer->getWindow();

    camera->keyControl(window->getKeysArray(), deltaTime);
    camera->mouseControl(window->getDeltaX(), window->getDeltaY());

    world->update();

    glm::vec3 chunk = *camera->getPosition();
    chunk.x = round(chunk.x / 16);
    chunk.y = round(chunk.y / 16);
    chunk.z = round(chunk.z / 16);

//    int SIZE = 16;
//    for (int i = -SIZE; i < SIZE; i++) {
//        for (int j = -4; j < 4; j++) {
//            for (int k = -SIZE; k < SIZE; k++) {
//                glm::vec3 adjustedPos(i + chunk.x, j + chunk.y, k + chunk.z);
//                world->genNewChunk(adjustedPos);
//            }
//        }
//    }
}

void GameInstance::draw() {
    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks()) {
        renderer->draw(chunk.second);
    }
}