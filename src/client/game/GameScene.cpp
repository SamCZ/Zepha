//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "../lua/l_register_block.h"
#include "../lua/l_register_blockmodel.h"

GameScene::GameScene(ClientState* state) :
        Scene(state),
        gui(state),
        debugGui() {

    server = new ServerConnection("127.0.0.1", 12345);
    server->init();

    textureAtlas = new TextureAtlas("../res/tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();

    //Register APIs here
    l_register_block(this, &p);
    l_register_blockmodel(this, &p);

    p.doFile("../res/lua/file.lua");

    //The scene requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    int SIZE = 10;
    int SIZEV = 4;
    for (int i = -SIZE; i < SIZE; i++) {
        for (int j = -SIZEV; j < SIZEV; j++) {
            for (int k = -SIZE; k < SIZE; k++) {
                world->genNewChunk(glm::vec3(i, j, k));
            }
        }
    }

    player = new Player();
    player->create(world, state->renderer->getCamera());
    player->setPos(glm::vec3(0, 22, 0));

    debugGui.pushGuiObjects(guiEntities);
    gui.pushGuiObjects(guiEntities);
}


void GameScene::update() {
    server->update(*player, playerEntities);

    auto window = state->renderer->getWindow();

    player->update(window->getKeysArray(), state->deltaTime, window->getDeltaX(), window->getDeltaY());

    debugGui.update(player, world, window, blockAtlas, state->fps);
    world->update();
}

void GameScene::draw() {
    state->renderer->begin();
    state->renderer->enableWorldShader();

    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks())
        state->renderer->draw(chunk.second);

    Texture* prevTexture = nullptr;

    for (auto &entity : entities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    //TEMPORARY
    for (auto &entity : playerEntities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    state->renderer->enableGuiShader();

    for (auto &entity : guiEntities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    state->renderer->end();
}

void GameScene::cleanup() {
    //TODO: Clean up
}